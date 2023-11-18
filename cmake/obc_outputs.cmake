macro(obc_add_outputs NAME)
    target_link_libraries(${NAME} pico_stdlib)

    pico_enable_stdio_usb(${NAME} 1)
    pico_enable_stdio_uart(${NAME} 0)
    pico_add_extra_outputs(${NAME})

    add_custom_command(
        TARGET ${NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NAME}>
            ${PROJECT_SOURCE_DIR}/executables/${NAME}.uf2
    )
endmacro()