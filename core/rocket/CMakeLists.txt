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

add_platform_output(${EXE_NAME})