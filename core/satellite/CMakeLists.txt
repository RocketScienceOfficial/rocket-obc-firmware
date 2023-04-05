set(EXE_NAME satellite)

add_executable(${EXE_NAME}
    main.c
    hw_config.c
    measurements_manager.c
)

add_pico_output(${EXE_NAME})