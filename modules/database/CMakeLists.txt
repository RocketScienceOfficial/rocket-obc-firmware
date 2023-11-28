obc_add_module(database dataman.c params.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal module_drivers_storage module_logger)