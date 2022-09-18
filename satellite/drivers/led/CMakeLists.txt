add_library(drivers_led_lib STATIC
    include/drivers/led/led_driver.h
    src/led_driver.c
    )

target_link_libraries(drivers_led_lib PUBLIC pico_stdlib utils_lib)
target_include_directories(drivers_led_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)