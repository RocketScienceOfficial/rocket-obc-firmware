add_executable(radio_module
    main.c
)

target_link_libraries(radio_module
    obc_hal
    lib_drivers_lora
)

obc_add_outputs(radio_module)