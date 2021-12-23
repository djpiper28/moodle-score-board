#include <stdio.h>
#include <stdlib.h>

typedef struct unit_test {
    int (*test_method)();
    const char *test_name;
} unit_test;

int self_test()
{
    return 1;
}

unit_test unit_tests[] = {
    {self_test, "test.c test"}
};

int test_func(int (*test_method)(), const char *test_name)
{
    int status;

    printf("Info: Testing %s:\n", test_name);

    status = test_method();
    if (status) {
        printf("Test-Pass: Test %s passed.\n", test_name);
    } else {
        printf("Test-Fail: Test %s failed.\n", test_name);
    }

    return status;
}

int run_tests()
{
    int tests, status, failed_count = 0;
    tests = sizeof(unit_tests) / sizeof(*unit_tests);

    for (int i = 0; i < tests; i++) {
        unit_test utest = unit_tests[i];
        status = test_func(utest.test_method, utest.test_name);
        if (!status) {
            failed_count++;
        }
    }

    printf("Info: ran %d tests\n", tests);
    return failed_count;
}

int main(int argc, char **argv)
{
    int status;

    if (argc > 1) {
        printf("Info: %d command line arguments were ignored.\n", argc - 1);
    }

    printf("Info: running tests...\n");
    status = run_tests();

    if (status != 0) {
        printf("Warn: %d tests failed, see output above.\n", status);
    }

    return status;
}

