#pragma once

#include "logging/logger.h"
#include "utils/time_tracker.h"

#define MY_TEST_FAIL_CODE 0
#define MY_TEST_SUCCESS_CODE 1

#define MY_TEST_INIT_FUNC_INTERNAL(name) int(name##_init)()
#define MY_TEST_INIT_FUNC(name) MY_TEST_INIT_FUNC_INTERNAL(name)
#define MY_TEST_FUNC_INTERNAL(name, num) int(name##_testCase_##num)()
#define MY_TEST_FUNC(name, num) MY_TEST_FUNC_INTERNAL(name, num)
#define MY_TEST_FUNC_DYNAMIC_INTERNAL(name, num) int(name##_testCase_dynamic_##num)()
#define MY_TEST_FUNC_DYNAMIC(name, num) MY_TEST_FUNC_DYNAMIC_INTERNAL(name, num)
#define MY_TEST_TERMINATE_FUNC_INTERNAL(name) int(name##_terminate)()
#define MY_TEST_TERMINATE_FUNC(name) MY_TEST_TERMINATE_FUNC_INTERNAL(name)
#define MY_TEST_END() return MY_TEST_SUCCESS_CODE

static int s_TestsStartTime;
static int s_FailedTests;
static int s_PassedTests;

#define MY_TEST_CASE(func)                                                      \
    {                                                                           \
        Timer __startTime = getMsSinceBoot();                                   \
        MY_LOG_CORE_INFO("Running test: %s", #func);                            \
        int __testResult = (func)();                                            \
        Timer __testDuration = getMsSinceBoot() - __startTime;                  \
        if (__testResult == MY_TEST_FAIL_CODE)                                  \
        {                                                                       \
            MY_LOG_CORE_ERROR("Test %s failed (%d ms)", #func, __testDuration); \
            s_FailedTests++;                                                    \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            MY_LOG_CORE_INFO("Test %s passed (%d ms)", #func, __testDuration);  \
            s_PassedTests++;                                                    \
        }                                                                       \
    }
#define MY_TEST_CASE_DYNAMIC(func)                  \
    if ((func)() == MY_TEST_FAIL_CODE)              \
    {                                               \
        MY_LOG_CORE_ERROR("Test %s failed", #func); \
    }
#define MY_TESTS_BEGIN()                      \
    {                                         \
        MY_LOG_CORE_INFO("Running tests..."); \
        s_TestsStartTime = getMsSinceBoot();  \
        s_FailedTests = 0;                    \
        s_PassedTests = 0;                    \
    }
#define MY_TESTS_END()                                                                            \
    {                                                                                             \
        MY_LOG_CORE_INFO("Finished running tests in %d ms", getMsSinceBoot() - s_TestsStartTime); \
        MY_LOG_CORE_INFO("Passed tests: %d", s_PassedTests);                                      \
        MY_LOG_CORE_INFO("Failed tests: %d", s_FailedTests);                                      \
    }
#define MY_TEST_CASE_INIT_FUNC_INTERNAL(name) MY_TEST_CASE((name##_init))
#define MY_TEST_CASE_INIT_FUNC(name) MY_TEST_CASE_INIT_FUNC_INTERNAL(name)
#define MY_TEST_CASE_FUNC_INTERNAL(name, num) MY_TEST_CASE((name##_testCase_##num))
#define MY_TEST_CASE_FUNC(name, num) MY_TEST_CASE_FUNC_INTERNAL(name, num)
#define MY_TEST_CASE_FUNC_DYNAMIC_INTERNAL(name, num) MY_TEST_CASE_DYNAMIC((name##_testCase_dynamic_##num))
#define MY_TEST_CASE_FUNC_DYNAMIC(name, num) MY_TEST_CASE_FUNC_DYNAMIC_INTERNAL(name, num)
#define MY_TEST_CASE_TERMINATE_FUNC_INTERNAL(name) MY_TEST_CASE((name##_terminate))
#define MY_TEST_CASE_TERMINATE_FUNC(name) MY_TEST_CASE_TERMINATE_FUNC_INTERNAL(name)

#define MY_ASSERT_MACRO_EXPAND(x) x

#define MY_INTERNAL_ASSERT_IMPL(x, ...) \
    if (!(x))                           \
    {                                   \
        MY_LOG_CORE_ERROR(__VA_ARGS__); \
        return MY_TEST_FAIL_CODE;       \
    }

#define MY_INTERNAL_ASSERT_NO_MSG(x) MY_ASSERT_MACRO_EXPAND(MY_INTERNAL_ASSERT_IMPL(x, "Assertion '%s' failed at '%s:%d'", #x, __FILE__, __LINE__))
#define MY_INTERNAL_ASSERT_WITH_MSG(x, msg) MY_ASSERT_MACRO_EXPAND(MY_INTERNAL_ASSERT_IMPL(x, "Assertion '%s' failed at '%s:%d' %s", #x, __FILE__, __LINE__, msg))
#define MY_INTERNAL_ASSERT_GET_MACRO(x, msg, macro, ...) macro
#define MY_ASSERT(...) MY_ASSERT_MACRO_EXPAND(MY_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__, MY_INTERNAL_ASSERT_WITH_MSG, MY_INTERNAL_ASSERT_NO_MSG)(__VA_ARGS__))