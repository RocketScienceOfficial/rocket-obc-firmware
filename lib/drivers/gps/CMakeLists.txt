obc_add_sublib(gps drivers ublox_neo_m9n_driver.c ublox_sam_m10q_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal lib_drivers_utils)