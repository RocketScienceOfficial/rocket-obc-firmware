add_library(drivers_gps_lib STATIC
    include/drivers/gps/u_blox_neo_m9n_driver.h
    src/u_blox_neo_m9n_driver.c
    )

target_include_directories(drivers_gps_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)