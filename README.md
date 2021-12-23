# moodle-score-board
## Dependancies
 - libcurl
 - orca
   - use the `make\_orca.sh` script to install if you have not done so already.
 - pthread

## Compile
Standard cmake process:
```bash
mkdir -p build && cd build
cmake .. && cmake --build . -j
ctest #run the unit tests
```

## Setup
Compile and then create a configuration file with `create_config_json.py`.

