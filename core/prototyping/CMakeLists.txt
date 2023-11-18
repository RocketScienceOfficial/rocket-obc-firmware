add_executable(prototyping
   main.c
)

target_link_libraries(prototyping
   module_drivers_hal
   module_drivers_accel
   module_drivers_adc
   module_drivers_barometer
   module_drivers_battery
   module_drivers_buzzer
   module_drivers_console
   module_drivers_gps
   module_drivers_hal
   module_drivers_igniter
   module_drivers_led
   module_drivers_lora
   module_drivers_magnetometer
   module_drivers_servo
   module_drivers_storage
)

obc_add_outputs(prototyping)