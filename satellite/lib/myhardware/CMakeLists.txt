add_library(hardware_lib STATIC
    include/myhardware/accelerometer.h
    include/myhardware/barometer.h
    include/myhardware/flash.h
    include/myhardware/gps.h
    include/myhardware/radio_protocol.h
    include/myhardware/radio.h
    include/myhardware/sd.h
    include/myhardware/servo.h
    src/accelerometer.c
    src/barometer.c
    src/flash.c
    src/gps.c
    src/radio_protocol.c
    src/radio.c
    src/sd.c
    src/servo.c
    )

target_link_libraries(hardware_lib PUBLIC utils_lib pico_stdlib hardware_i2c hardware_spi hardware_flash hardware_interp hardware_sync hardware_pwm FatFs_SPI)
target_include_directories(hardware_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)