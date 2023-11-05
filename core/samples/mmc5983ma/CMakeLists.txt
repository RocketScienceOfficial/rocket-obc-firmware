set(MODULE_NAME samples)
set(LIB_NAME mmc5983ma)
set(EXE_NAME "${MODULE_NAME}_${LIB_NAME}")

add_executable(${EXE_NAME}
    main.c
)

add_platform_output(${EXE_NAME})