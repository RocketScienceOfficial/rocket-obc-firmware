add_library(drivers_accelerometer_lib STATIC
    include/drivers/accelerometer/bmi088_driver.h
    include/drivers/accelerometer/mpu6050_driver.h
    src/bmi088_driver.c
    src/mpu6050_driver.c
    )

target_link_libraries(drivers_accelerometer_lib PUBLIC pico_stdlib hardware_i2c utils_lib drivers_hwutils_lib)
target_include_directories(drivers_accelerometer_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)