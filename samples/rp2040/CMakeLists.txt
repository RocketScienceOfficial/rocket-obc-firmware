set(MODULE_NAME samples)
set(LIB_NAME rp2040)
set(EXE_NAME "${MODULE_NAME}_${LIB_NAME}")

add_executable(${EXE_NAME}
    main.c
)

target_link_libraries(${EXE_NAME} PRIVATE pico_stdlib)

add_pico_output(${EXE_NAME})