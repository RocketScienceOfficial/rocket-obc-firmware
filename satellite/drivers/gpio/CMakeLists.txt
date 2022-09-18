add_library(drivers_gpio_lib STATIC
    include/drivers/gpio/adc_driver.h
    include/drivers/gpio/i2c_driver.h
    include/drivers/gpio/spi_driver.h
    src/adc_driver.c
    src/i2c_driver.c
    src/spi_driver.c
)

target_link_libraries(drivers_gpio_lib PUBLIC pico_stdlib hardware_i2c)
target_include_directories(drivers_gpio_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)