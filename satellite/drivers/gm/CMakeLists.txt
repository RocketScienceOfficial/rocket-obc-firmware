add_library(drivers_gm_lib STATIC
    include/drivers/gm/sts_5_driver.h
    src/sts_5_driver.c
    )

target_link_libraries(drivers_gm_lib PUBLIC utils_lib drivers_hwutils_lib)
target_include_directories(drivers_gm_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)