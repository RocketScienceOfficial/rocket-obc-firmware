add_library(drivers_hwutils_lib STATIC
    include/drivers/hwutils/errors_types.h
    include/drivers/hwutils/pinout_utils.h
    src/pinout_utils.c
    )

target_include_directories(drivers_hwutils_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)