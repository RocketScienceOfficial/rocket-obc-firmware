add_library(drivers_battery_lib STATIC
    include/drivers/battery/battery_driver.h
    src/battery_driver.c
    )

target_link_libraries(drivers_battery_lib PUBLIC pico_stdlib utils_lib)
target_include_directories(drivers_battery_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)