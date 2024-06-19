obc_add_sublib(barometer drivers ms5607_driver.c ms5611_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal lib_drivers_utils)