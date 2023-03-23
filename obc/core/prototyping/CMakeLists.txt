set(EXE_NAME prototyping)

add_executable(${EXE_NAME}
    main.c
    hw_config.c
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)

pico_generate_pio_header(${EXE_NAME} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

target_link_libraries(${EXE_NAME} PRIVATE pico_stdlib core_common_lib hardware_i2c hardware_uart hardware_spi hardware_pwm hardware_adc hardware_pio)

pico_enable_stdio_usb(${EXE_NAME} 1)
pico_enable_stdio_uart(${EXE_NAME} 0)
pico_add_extra_outputs(${EXE_NAME})