add_library(maths
    kalman_filter.h
    kalman_filter.c
    constants.h
    )

target_include_directories(maths PUBLIC .)