set(MODULE_NAME samples)
set(LIB_NAME sd)
set(EXE_NAME "${MODULE_NAME}_${LIB_NAME}")

add_executable(${EXE_NAME}
    main.c
    hw_config.c
)

add_platform_output(${EXE_NAME})