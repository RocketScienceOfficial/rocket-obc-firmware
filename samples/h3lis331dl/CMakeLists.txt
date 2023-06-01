set(MODULE_NAME samples)
set(LIB_NAME h3lis331dl)
set(EXE_NAME "${MODULE_NAME}_${LIB_NAME}")

add_executable(${EXE_NAME}
    main.c
)

add_pico_output(${EXE_NAME})