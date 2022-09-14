add_library(drivers_uv_lib STATIC
    include/drivers/uv/guva_s12d_driver.h
    src/guva_s12d_driver.c
    )

target_link_libraries(drivers_uv_lib PUBLIC pico_stdlib utils_lib)
target_include_directories(drivers_uv_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)