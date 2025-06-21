/*
  * tinytest.c - tinytest-c is a lightweight, minimal, and simple testing framework
  *              for the C programming language.
  *
  * Version: v2.0
  * Repository: https://github.com/9takoyakii/tinytest-c
  *
  * SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tinytest.h"

#define NOT_INITIALIZED_ERROR_MESSAGE "Run the ttest_init/INIT function first"
#define ERROR(msg) printf(">> ERROR @ " msg "\n"); \
    exit(1)
#define TEST_SUITE_MAX_DEPTH 4

static ttest_cleanupFunc *rootcleanupFunc = NULL;
static int initialized = 0;
static int concluded = 0;
static int totalPass;
static int totalFail;
static int totalSkip;
static long int totalDuration;
static struct ttest_TestSuiteStack testSuiteStack = {
    .cap = TEST_SUITE_MAX_DEPTH,
    .len = 0,
    .nextPtr = NULL,
};

void ttest_init(void) {
    if (initialized) {
        ERROR("Don't call the ttest_init / INIT twice");
    }

    initialized = 1;
    testSuiteStack.len = 0;
    testSuiteStack.nextPtr = calloc(TEST_SUITE_MAX_DEPTH, sizeof(struct ttest_TestSuite));
}

void ttest_cleanUp(ttest_cleanupFunc *cleanFunc, int onlyForThis) {
    if (testSuiteStack.len == 0) {
        rootcleanupFunc = cleanFunc;
    } else {
        struct ttest_TestSuite *currTestSuite = testSuiteStack.nextPtr - 1;
        if (currTestSuite->test.status != ttest_NONE) {
            ERROR("Don't call ttest_cleanUp / CLEANUP when test is running");
        }
        currTestSuite->cleanup.cleanupFunc = cleanFunc;
        currTestSuite->cleanup.onlyForThis = !!onlyForThis;
    }
}

void ttest_beginTestSuite(const char *desc, int skip) {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (testSuiteStack.len >= testSuiteStack.cap) {
        ERROR("Your test suite exceeds the allowed depth level, the maximum depth is 4");
    }

    if (testSuiteStack.len == 0) {
        if (skip) {
            printf("\n@@@@ %s # SKIP\n", desc);
            return;
        } else {
            printf("\n@@@@ %s\n", desc);
        }
    } else {
        if ((testSuiteStack.nextPtr - 1)->test.status != ttest_NONE) {
            ERROR("There are still tests running, unable to create a new test suite");
        }

        if (skip) {
            printf("\n@@@@ %s :: %s # SKIP\n", (testSuiteStack.nextPtr - 1)->desc, desc);
            return;
        } else {
            printf("\n@@@@ %s :: %s\n", (testSuiteStack.nextPtr - 1)->desc, desc);
        }
    }

    testSuiteStack.nextPtr->desc = desc;
    testSuiteStack.nextPtr->totalSkip = 0;
    testSuiteStack.nextPtr->totalFail = 0;
    testSuiteStack.nextPtr->totalPass = 0;
    testSuiteStack.nextPtr->test.status = ttest_NONE;
    testSuiteStack.nextPtr->totalDuration = 0;
    testSuiteStack.nextPtr++;
    testSuiteStack.len++;
}

// endTestSuite will output the results to STDOUT and also add statistical information
// to the previous testSuite or parentTestSuite
void ttest_endTestSuite(void) {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (testSuiteStack.len == 0) {
        ERROR("No test suite running");
    }


    struct ttest_TestSuite *currTestSuite = testSuiteStack.nextPtr - 1;
    if (currTestSuite->test.status != ttest_NONE) {
        ERROR("There are still tests running, unable to cease current test suite");
    }

    if (testSuiteStack.len == 1) {
        printf("\n!!!! %s # DONE :: %d pass, %d fail, %d skip (%lums) !!!!\n\n",
            currTestSuite->desc,
            currTestSuite->totalPass,
            currTestSuite->totalFail,
            currTestSuite->totalSkip,
            currTestSuite->totalDuration
        );

        totalPass += currTestSuite->totalPass;
        totalFail += currTestSuite->totalFail;
        totalSkip += currTestSuite->totalSkip;
        totalDuration += currTestSuite->totalDuration;
    } else {
        struct ttest_TestSuite *parentTestSuite = testSuiteStack.nextPtr - 2;
        parentTestSuite->totalPass += currTestSuite->totalPass;
        parentTestSuite->totalFail += currTestSuite->totalFail;
        parentTestSuite->totalSkip += currTestSuite->totalSkip;
        parentTestSuite->totalDuration += currTestSuite->totalDuration;

        printf("==== %s :: %s # DONE (%lums)\n",
            (currTestSuite - 1)->desc,
            currTestSuite->desc,
            currTestSuite->totalDuration
        );
    }

    testSuiteStack.nextPtr--;
    testSuiteStack.len--;
}

void ttest_beginTest(const char *desc, int failAsPassFlag, int skip) {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (testSuiteStack.len == 0) {
        ERROR("No test suite running");
    }

    struct ttest_TestSuite *currTestSuite = testSuiteStack.nextPtr - 1;
    if (currTestSuite->test.status != ttest_NONE) {
        ERROR("There is a test running, can only run 1 test at a time");
    }

    if (skip) {
        printf("---- %s ::> %s\n      # SKIP (0ms)\n",
            currTestSuite->desc,
            desc
        );
        currTestSuite->totalSkip++;
        return;
    }

    currTestSuite->test.desc = desc;
    currTestSuite->test.failAsPassFlag = failAsPassFlag;
    currTestSuite->test.status = ttest_PASS;
    currTestSuite->test.startClock = clock();
    printf("---- %s ::> %s\n",
        currTestSuite->desc,
        desc
    );
}

void ttest_endTest(void) {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (testSuiteStack.len == 0) {
        ERROR("No test suite running");
    }

    struct ttest_TestSuite *currTestSuite = testSuiteStack.nextPtr - 1;
    if (currTestSuite->test.status == ttest_NONE) {
        ERROR("No test is running, call ttest_endTest if there is a test running");
    }

    size_t i = 1;
    for (; i <= testSuiteStack.len; i++) {
        struct ttest_TestSuite *t = testSuiteStack.nextPtr - i;
        if (t->cleanup.cleanupFunc != NULL && !(i != 1 && t->cleanup.onlyForThis)) {
            t->cleanup.cleanupFunc(&currTestSuite->test);
        }
    }

    if (rootcleanupFunc != NULL) {
        rootcleanupFunc(&currTestSuite->test);
    }

    enum ttest_TEST_STATUS status = currTestSuite->test.status;
    long int duration = clock() - currTestSuite->test.startClock;
    currTestSuite->totalDuration += duration;
    currTestSuite->test.status = ttest_NONE;

    switch (status) {
        case ttest_PASS:
            printf("      # PASS (%lums)\n", duration);
            currTestSuite->totalPass++;
            break;
        default:
            printf("      # FAIL (%lums)\n", duration);
            currTestSuite->totalFail++;
    }
}

void ttest_test(const char *desc, int failAsPassFlag, int skip, ttest_testFunc test) {
    ttest_beginTest(desc, failAsPassFlag, skip);
    if (!skip) {
        // NOTE: || failAsPassFlag is added to prevent failure if return 0
        //       (because if failAsPassFlag = 1 then if false (0) will fail).
        ASSERT_N(test(&(testSuiteStack.nextPtr - 1)->test) || failAsPassFlag);
        ttest_endTest();
    }
}

int ttest_conclude(void) {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (concluded) {
        ERROR("Don't call the ttest_conclude / CONCLUDE twice");
    }

    if (testSuiteStack.len != 0) {
        ERROR("There is still a test suite running, call the ttest_conclude / CONCLUDE function only when all test suites have finished");
    }

    long int duration = totalDuration;
    printf("--- %d pass, %d fail, %d skip (%lums) ---\n\n",
        totalPass,
        totalFail,
        totalSkip,
        duration
    );

    concluded = 1;
    free(testSuiteStack.nextPtr);

    return !!totalFail;
}

int ttest_assert(int expr) {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (testSuiteStack.len == 0) {
        ERROR("No test suite running");
    }

    struct ttest_TestSuite *currTestSuite = testSuiteStack.nextPtr - 1;
    if (currTestSuite->test.status == ttest_NONE) {
        ERROR("No test is running, call ttest_assert / ASSERT if there is a test running");
    }

    // isPass contains whether ASSERT is considered PASS or FAIL
    // based on expr and failAsPassFlag
    int isPass = expr ^ currTestSuite->test.failAsPassFlag;
    currTestSuite->test.status = (isPass && currTestSuite->test.status == ttest_PASS)
        ? ttest_PASS
        : ttest_FAIL;
    return expr;
}
