add_library(drivers_barometer_lib STATIC
    include/drivers/barometer/bme688_driver.h
    include/drivers/barometer/bmp280_driver.h
    src/bme688_driver.c
    src/bmp280_driver.c
    )

target_link_libraries(drivers_barometer_lib PUBLIC pico_stdlib hardware_i2c utils_lib)
target_include_directories(drivers_barometer_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)