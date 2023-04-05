set(EXE_NAME prototyping)

add_executable(${EXE_NAME}
    main.c
)

target_link_libraries(${EXE_NAME} PRIVATE pico_stdlib core_common_lib hardware_i2c hardware_uart hardware_spi hardware_pwm hardware_adc hardware_pio)

add_pico_output(${EXE_NAME})