set(EXE_NAME rocket)

add_executable(${EXE_NAME}
    main.c
    measurements_utils.c
    radio_utils.c
    commands.c
    saver.c
    algorithms.c
)

add_platform_output(${EXE_NAME})