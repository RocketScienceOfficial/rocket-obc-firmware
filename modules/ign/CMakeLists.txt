obc_add_module(ign ign_controller.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal module_flight_sm module_logger module_maths)