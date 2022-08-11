add_executable(prototyping
    main.c
)

target_link_libraries(prototyping PRIVATE core_shared)

pico_enable_stdio_usb(prototyping 1)
pico_enable_stdio_uart(prototyping 0)
pico_add_extra_outputs(prototyping)