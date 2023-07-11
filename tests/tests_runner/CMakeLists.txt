set(EXE_NAME tests_runner)

add_executable(${EXE_NAME}
    main.c
)

target_link_libraries(${EXE_NAME} PRIVATE drivers_lib tests_drivers tests_services)

add_platform_output(${EXE_NAME})