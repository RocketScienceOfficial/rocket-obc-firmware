obc_add_sublib(led drivers simple_led_driver.c w2812_driver.c)

if(OBC_PLATFORM STREQUAL rp2040)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
    pico_generate_pio_header(lib_drivers_led ${CMAKE_CURRENT_LIST_DIR}/src/ws2812_driver_pico.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

    target_link_libraries(${LIB_NAME} INTERFACE pico_stdlib hardware_pio)
endif()

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)