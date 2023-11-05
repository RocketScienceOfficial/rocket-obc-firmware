set(EXE_NAME rocket)

add_executable(${EXE_NAME}
    main.c
    measurements.c
    radio_utils.c
    commands.c
    saver.c
    algorithms.c
    status.c
    mission_control.c
    voltage_control.c
    ign.c
)

target_compile_definitions(${EXE_NAME} PRIVATE
    PICO_FLASH_SIZE_BYTES=16777216
)

target_link_libraries(${EXE_NAME} PRIVATE pico_multicore)

add_platform_output(${EXE_NAME})