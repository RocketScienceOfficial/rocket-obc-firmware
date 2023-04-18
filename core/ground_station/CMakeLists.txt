set(EXE_NAME ground_station)

add_executable(${EXE_NAME}
    main.c
)

add_pico_output(${EXE_NAME})