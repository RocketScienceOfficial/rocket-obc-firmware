macro(obc_add_platform_output NAME)
    pico_enable_stdio_usb(${NAME} 1)
    pico_enable_stdio_uart(${NAME} 0)
    pico_add_extra_outputs(${NAME})

    add_custom_command(
        TARGET ${NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NAME}>
            ${PROJECT_SOURCE_DIR}/executables/${NAME}
    )
endmacro()