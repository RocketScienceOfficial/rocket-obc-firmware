set(MODULE_NAME drivers)
set(LIB_NAME led)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/simple_led_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/w2812_driver.h
    src/simple_led_driver.c
    src/w2812_driver.c
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)

pico_generate_pio_header(${LIB_GENERAL_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/ws2812_driver.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

target_link_libraries(${LIB_GENERAL_NAME} PUBLIC drivers_gpio_lib hardware_pio)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)