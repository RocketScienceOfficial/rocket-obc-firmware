obc_add_lib(radiolink radiolink.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal lib_maths lib_geo lib_crypto)