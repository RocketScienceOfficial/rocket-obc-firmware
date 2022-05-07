add_library(myassert INTERFACE 
    my_assert.h
    )

target_link_libraries(myassert INTERFACE console_colors)

target_include_directories(myassert INTERFACE .)