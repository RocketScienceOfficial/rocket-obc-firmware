set(EXE_NAME ground_station)

add_executable(${EXE_NAME}
    main.c
    receiver_logger.c
)

add_pico_output(${EXE_NAME})