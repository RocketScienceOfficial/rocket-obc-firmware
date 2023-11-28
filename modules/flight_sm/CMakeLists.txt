obc_add_module(flight_sm flight_sm_control.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_maths)