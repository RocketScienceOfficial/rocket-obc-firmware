add_library(myassert INTERFACE 
    my_assert.h
    )

target_include_directories(myassert INTERFACE .)