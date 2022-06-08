#pragma once
#include "logger.h"

static int s_TestFailed = 0;

#define MY_TEST_CASE(name, func) MY_CORE_LOG_INFO("Running test: %s", #name); func(); if (s_TestFailed) { MY_CORE_LOG_ERROR("Test %s failed", #name); } else { MY_CORE_LOG_INFO("Test %s passed", #name); } s_TestFailed = 0;

#define MY_INTERNAL_ASSERT_EXPAND(x) x
#define MY_INTERNAL_ASSERT_IMPL(x, ...) if (!(x)) { s_TestFailed = 1; MY_LOG_CORE_ERROR(__VA_ARGS__); }
#define MY_INTERNAL_ASSERT_NO_MSG(x) MY_INTERNAL_ASSERT_EXPAND(MY_INTERNAL_ASSERT_IMPL(x, "Assertion '%s' failed at %s:%d", #x, __FILE__, __LINE__))
#define MY_INTERNAL_ASSERT_WITH_MSG(x, msg) MY_INTERNAL_ASSERT_EXPAND(MY_INTERNAL_ASSERT_IMPL(x, "Assertion '%s' failed at %s:%d %s", #x, __FILE__, __LINE__, msg))
#define MY_INTERNAL_ASSERT_GET_MACRO(x, msg, macro, ...) macro
#define MY_ASSERT(...) MY_INTERNAL_ASSERT_EXPAND(MY_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__, MY_INTERNAL_ASSERT_WITH_MSG, MY_INTERNAL_ASSERT_NO_MSG)(__VA_ARGS__))