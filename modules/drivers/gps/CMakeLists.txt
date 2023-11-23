obc_add_submodule(gps drivers u_blox_neo_m9n_driver.c u_blox_sam_m10q_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal module_drivers_utils)