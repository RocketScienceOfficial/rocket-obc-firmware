add_library(drivers_lib main.c)

target_link_libraries(drivers_lib INTERFACE
    drivers_accelerometer_lib
    drivers_barometer_lib
    drivers_battery_lib
    drivers_flash_lib
    drivers_gm_lib
    drivers_gps_lib
    drivers_led_lib
    drivers_lora_lib
    drivers_sd_lib
    drivers_servo_lib
    drivers_uv_lib
)