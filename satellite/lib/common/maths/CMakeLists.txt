add_library(maths
    constants.h
    cryptography.h
    cryptography.c
    )

target_link_libraries(maths logging)

target_include_directories(maths PUBLIC .)