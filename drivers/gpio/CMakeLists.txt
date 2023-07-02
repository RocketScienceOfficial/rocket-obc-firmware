set(MODULE_NAME drivers)
set(LIB_NAME gpio)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/adc_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/gpio_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/i2c_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/pwm_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/spi_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/uart_driver.h
    src/adc_driver.c
    src/gpio_driver.c
    src/i2c_driver.c
    src/pwm_driver.c
    src/spi_driver.c
    src/uart_driver.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE hardware_adc hardware_i2c hardware_pwm hardware_spi hardware_uart)
target_link_libraries(${LIB_GENERAL_NAME} PUBLIC drivers_tools_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)