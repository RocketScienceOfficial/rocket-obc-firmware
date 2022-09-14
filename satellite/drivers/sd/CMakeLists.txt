add_library(drivers_sd_lib STATIC
    include/drivers/sd/sd_driver.h
    src/sd_driver.c
    )

target_link_libraries(drivers_sd_lib PUBLIC pico_stdlib FatFs_SPI utils_lib)
target_include_directories(drivers_sd_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)