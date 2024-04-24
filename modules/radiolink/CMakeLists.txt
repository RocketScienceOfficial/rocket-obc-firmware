obc_add_module(radiolink radiolink.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_maths module_geo module_crypto module_logger)