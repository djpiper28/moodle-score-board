cmake_minimum_required(VERSION 3.12)
set(VERSION "0.0.1")

project(moodle-score VERSION ${VERSION} LANGUAGES C)
set(REPO_URL "https://github.com/djpiper28/moodle-score-board")

message(STATUS "Building ${CMAKE_PROJECT_NAME}")
message(STATUS "Git repo: ${REPO_URL}")
message(STATUS "Build type is ${CMAKE_BUILD_TYPE} for version ${CMAKE_PROJECT_VERSION}")

# Detect OS
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  message (STATUS "Operating system detected: Linux")
  set (LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  message (STATUS "Operating system detected: OSX")
  set (OSX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message (STATUS "Operating system detected: Windows")
  set (WINDOWS TRUE)
else ()
  message (ERROR "Operating system not recognised. Expected Windows, Linux or Darwin, but found ${CMAKE_SYSTEM_NAME}")
endif ()

# specify the C++ standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# set the project name
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Format the code
execute_process(
  COMMAND bash format.sh
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")

# compiler flags
set(COMPILER_FLAGS "-Wall -Wextra -Wpedantic")
add_definitions("-DVERSION=\"${VERSION} (${GIT_COMMIT_HASH}@${GIT_BRANCH})\"")
add_definitions("-DOS=\"${CMAKE_SYSTEM_NAME}\"")
add_definitions("-DREPO_URL=\"${REPO_URL}\"")

set(CMAKE_C_FLAGS_DEBUG "-g -O0 ${COMPILER_FLAGS} -DDEBUG -Wno-unused-parameter")
set(CMAKE_C_FLAGS_RELEASE "-O2 ${COMPILER_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake)

if (CMAKE_BUILD_TYPE STREQUAL "TEST")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG} -DTEST")

    set(CMAKE_C_COMPILER "gcc")

    include(CodeCoverage)

    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_xml(NAME coverage
        EXECUTABLE moodle-score-tests
        EXCLUDE "build/*" "test/*" "src/main.c")

endif()

# src
set(MAIN
    src/main.c
    )
set(SRC_FILES
    src/config.c
    )

set(LIBS curl ncurses pthread discord crypto ssl m)

add_executable(moodle-score ${SRC_FILES} ${MAIN})

file(GLOB STATIC_IMG
  "assets/**"
)
file(COPY ${STATIC_IMG} DESTINATION "${CMAKE_BINARY_DIR}/assets/")
file(COPY "config.json" DESTINATION "${CMAKE_BINARY_DIR}")

target_link_libraries(moodle-score ${LIBS})
target_include_directories(moodle-score PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR})

if (CMAKE_BUILD_TYPE STREQUAL "TEST")
    # tests, set the flags manually
    set(CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_DEBUG})

    set (TEST_FILES
        ${SRC_FILES}
        test/test.c
        )
    add_executable(moodle-score-tests ${TEST_FILES})
    target_link_libraries(moodle-score-tests
        cppunit ${LIBS})
    target_include_directories(moodle-score-tests PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR})

    enable_testing ()
    add_test(NAME moodle-score-tests COMMAND moodle-score-tests)
		add_test(NAME moodle-score-tests-memcheck COMMAND python3 ${CMAKE_SOURCE_DIR}/mem_test.py)
endif()
