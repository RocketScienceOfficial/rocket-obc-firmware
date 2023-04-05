set(EXE_NAME tests_runner)

add_executable(${EXE_NAME}
    main.c
)

target_link_libraries(${EXE_NAME} PRIVATE drivers_console_lib tests_crypto tests_drivers tests_kernel)

add_pico_output(${EXE_NAME})