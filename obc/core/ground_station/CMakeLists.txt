set(EXE_NAME ground_station)

add_executable(${EXE_NAME}
    main.c
    receiver_logger.c
)

target_link_libraries(${EXE_NAME} PRIVATE core_common_lib)

pico_enable_stdio_usb(${EXE_NAME} 1)
pico_enable_stdio_uart(${EXE_NAME} 0)
pico_add_extra_outputs(${EXE_NAME})