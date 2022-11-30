set(EXE_NAME prototyping)

add_executable(${EXE_NAME}
    main.c
)

target_link_libraries(${EXE_NAME} PRIVATE pico_stdlib core_common_lib hardware_i2c hardware_uart hardware_spi hardware_pwm hardware_adc)

pico_enable_stdio_usb(${EXE_NAME} 1)
pico_enable_stdio_uart(${EXE_NAME} 0)
pico_add_extra_outputs(${EXE_NAME})