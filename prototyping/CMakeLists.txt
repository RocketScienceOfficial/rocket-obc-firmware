add_executable(prototyping
   main.c
)

target_link_libraries(prototyping
   obc_hal
   lib_drivers_accel
   lib_drivers_adc
   lib_drivers_barometer
   lib_drivers_buzzer
   lib_drivers_gps
   lib_drivers_imu
   lib_drivers_led
   lib_drivers_lora
   lib_drivers_magnetometer
   lib_drivers_servo
   lib_drivers_storage
   lib_nmea
   lib_ahrs
   lib_battery
   lib_radiolink
   lib_maths
   lib_geo
   lib_crypto
)

obc_add_outputs(prototyping)