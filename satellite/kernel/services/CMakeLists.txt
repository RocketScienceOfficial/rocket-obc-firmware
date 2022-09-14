add_library(kernel_services_lib STATIC
    include/kernel/services/radio_protocol.h
    src/radio_protocol.c
    )

target_link_libraries(kernel_services_lib PRIVATE pico_stdlib utils_lib drivers_lib)
target_include_directories(kernel_services_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)