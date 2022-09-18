add_library(drivers_servo_lib STATIC
    include/drivers/servo/mg995_driver.h
    src/mg995_driver.c
)

target_link_libraries(drivers_servo_lib PUBLIC pico_stdlib hardware_pwm utils_lib drivers_hwutils_lib)
target_include_directories(drivers_servo_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)