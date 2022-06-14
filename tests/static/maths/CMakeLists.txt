add_library(maths_tests test_maths.h test_maths.c)

target_link_libraries(maths_tests PUBLIC maths test_framework)

target_include_directories(maths_tests PUBLIC .)