set(MODULE_NAME drivers)
set(LIB_NAME display)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/lcd_16x2_i2c_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/ssd1306_driver.h
    src/lcd_16x2_i2c_driver.c
    src/ssd1306_driver.c
    )

target_link_libraries(${LIB_GENERAL_NAME} PUBLIC drivers_gpio_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)