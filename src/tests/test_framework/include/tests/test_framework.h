#ifndef _TEST_FRAMEWORK_H
#define _TEST_FRAMEWORK_H

#include <stdbool.h>
#include <hal/serial_driver.h>
#include <hal/time_tracker.h>
#include <hal/board_control.h>

static hal_msec_t s_TestsStartTime;
static hal_msec_t s_SectionStartTime;
static int s_PassedTests;
static int s_FailedTests;
static bool s_IsTestError;
static const char *s_CurrentSection;

#ifndef TEST_MODULE_NAME
#define TEST_MODULE_NAME ""
#endif

#define __MACRO_STR(x) #x

#define TEST_START()                                            \
    hal_board_init(5000, true);                                 \
    hal_serial_printf("Starting test: %s\n", TEST_MODULE_NAME); \
    s_TestsStartTime = hal_time_get_ms_since_boot();

#define TEST_END()                                                                                                                                                                                                                                          \
    hal_serial_printf("\n%d%% tests passed, %d tests failed out of %d   %.3f sec\n", s_PassedTests * 100 / (s_PassedTests + s_FailedTests), s_FailedTests, s_PassedTests + s_FailedTests, (float)(hal_time_get_ms_since_boot() - s_TestsStartTime) / 1000); \
    while (true)                                                                                                                                                                                                                                            \
    {                                                                                                                                                                                                                                                       \
        hal_board_tight_loop();                                                                                                                                                                                                                             \
    };

#define TEST_START_SECTION(name)                       \
    s_SectionStartTime = hal_time_get_ms_since_boot(); \
    s_CurrentSection = name;

#define TEST_END_SECTION()                                                                                                                                                                                                 \
    if (s_IsTestError)                                                                                                                                                                                                     \
    {                                                                                                                                                                                                                      \
        s_FailedTests++;                                                                                                                                                                                                   \
    }                                                                                                                                                                                                                      \
    else                                                                                                                                                                                                                   \
    {                                                                                                                                                                                                                      \
        s_PassedTests++;                                                                                                                                                                                                   \
    }                                                                                                                                                                                                                      \
    hal_serial_printf("Test #%d: %s  ........  %s   %.3f sec\n", s_PassedTests + s_FailedTests, s_CurrentSection, s_IsTestError ? "Failed" : "Passed", (float)(hal_time_get_ms_since_boot() - s_SectionStartTime) / 1000); \
    s_IsTestError = false;

#define TEST_ASSERT(x)                                                                               \
    if (!(x))                                                                                        \
    {                                                                                                \
        hal_serial_printf("Assertion '%s' failed at '%s:%d'\n", __MACRO_STR(x), __FILE__, __LINE__); \
        s_IsTestError = true;                                                                        \
    }

#endif