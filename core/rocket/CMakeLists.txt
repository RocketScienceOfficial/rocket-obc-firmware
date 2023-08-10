set(EXE_NAME rocket)

add_executable(${EXE_NAME}
    main.c
    measurements_utils.c
    radio_utils.c
)

add_platform_output(${EXE_NAME})