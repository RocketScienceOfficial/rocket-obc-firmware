set(EXE_NAME satellite)

add_executable(${EXE_NAME}
    main.c
)

add_pico_output(${EXE_NAME})