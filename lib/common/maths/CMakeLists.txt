add_library(maths
    kalman_filter.h
    kalman_filter.c
    constants.h
    cryptography.h
    cryptography.c
    )

target_link_libraries(maths logging)

target_include_directories(maths PUBLIC .)