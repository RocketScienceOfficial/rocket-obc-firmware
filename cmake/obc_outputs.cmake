macro(obc_add_outputs NAME)
    if(OBC_PLATFORM STREQUAL pico)
        target_link_libraries(${NAME} pico_stdlib)

        pico_enable_stdio_usb(${NAME} 1)
        pico_enable_stdio_uart(${NAME} 0)
        pico_add_extra_outputs(${NAME})
    endif()
endmacro()

macro(obc_add_test NAME)
    add_executable(${NAME} ${NAME}.c)

    target_link_libraries(${NAME} test_framework ${ARGN})
    target_compile_definitions(${NAME} PRIVATE TEST_MODULE_NAME="${NAME}")

    obc_add_outputs(${NAME})
endmacro()