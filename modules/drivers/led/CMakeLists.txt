obc_add_submodule(led drivers simple_led_driver.c w2812_driver.c)

if(OBC_PLATFORM STREQUAL pico)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
    pico_generate_pio_header(module_drivers_led ${CMAKE_CURRENT_LIST_DIR}/src/ws2812_driver_pico.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

    target_link_libraries(${MODULE_NAME} INTERFACE pico_stdlib hardware_pio)
endif()

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal)