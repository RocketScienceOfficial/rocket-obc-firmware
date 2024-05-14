obc_add_module(ign ign_controller.c)

target_link_libraries(${MODULE_NAME} INTERFACE obc_hal module_sm lib_maths)