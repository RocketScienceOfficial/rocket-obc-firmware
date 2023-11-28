obc_add_submodule(gps drivers ublox_neo_m9n_driver.c ublox_sam_m10q_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal module_drivers_utils)