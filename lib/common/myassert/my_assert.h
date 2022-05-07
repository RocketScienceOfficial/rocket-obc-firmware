#pragma once

#include <stdlib.h>
#include "console_colors.h"

#define MY_ASSERT_EXPAND_INTERNAL(x) x
#define MY_INTERNAL_ASSERT_IMPL(x, ...) if (!(x)) { setConsoleForegroundColor(RED); panic(__VA_ARGS__); }
#define MY_INTERNAL_ASSERT_NO_MSG(x) MY_ASSERT_EXPAND_INTERNAL(MY_INTERNAL_ASSERT_IMPL(x, "Assertion '%s' failed at %s:%d", #x, __FILE__, __LINE__))
#define MY_INTERNAL_ASSERT_WITH_MSG(x, msg) MY_ASSERT_EXPAND_INTERNAL(MY_INTERNAL_ASSERT_IMPL(x, "Assertion '%s' failed at %s:%d '%s'", #x, __FILE__, __LINE__, msg))
#define MY_INTERNAL_ASSERT_GET_MACRO(x, msg, macro, ...) macro
#define MY_ASSERT(...) MY_ASSERT_EXPAND_INTERNAL(MY_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__, MY_INTERNAL_ASSERT_WITH_MSG, MY_INTERNAL_ASSERT_NO_MSG)(__VA_ARGS__))