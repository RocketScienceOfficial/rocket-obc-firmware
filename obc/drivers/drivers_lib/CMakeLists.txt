set(LIB_GENERAL_NAME drivers_lib)

add_library(${LIB_GENERAL_NAME} main.c)

target_link_libraries(${LIB_GENERAL_NAME} INTERFACE
    drivers_accelerometer_lib
    drivers_barometer_lib
    drivers_battery_lib
    drivers_console_lib
    drivers_display_lib
    drivers_gps_lib
    drivers_led_lib
    drivers_lora_lib
    drivers_potentiometer_lib
    drivers_servo_lib
    drivers_storage_lib
    drivers_tempr_lib
    drivers_uv_lib
)