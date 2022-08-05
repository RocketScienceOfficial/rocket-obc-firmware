add_library(maths_lib
    constants.h
    cryptography.h
    cryptography.c
    )

target_include_directories(maths_lib PUBLIC .)