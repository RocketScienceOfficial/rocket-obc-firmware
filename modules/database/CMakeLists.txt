obc_add_module(database dataman.c params.c)

target_link_libraries(${MODULE_NAME} INTERFACE obc_hal lib_drivers_storage lib_maths)