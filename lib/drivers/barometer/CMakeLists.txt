obc_add_sublib(barometer drivers ms56xx_02ba03.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal lib_drivers_utils)