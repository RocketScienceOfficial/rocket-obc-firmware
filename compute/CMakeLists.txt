cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(compute C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME} 
    main.c
    hw_config.c
    bmp280_controller.c
    logger.c
    sd_card_controller.c
    mpu6050_controller.c
    kalman.c
)

add_subdirectory(lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_i2c
    hardware_pwm
    hardware_clocks
    FatFs_SPI
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})