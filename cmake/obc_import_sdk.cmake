macro(obc_include_sdk)
    if(OBC_PLATFORM STREQUAL rp2040)
        set(PICO_BOARD_HEADER_DIRS ${CMAKE_SOURCE_DIR}/platform/rp2040/boards/${OBC_BOARD})
        set(PICO_BOARD ${OBC_BOARD})

        include(lib/pico-sdk/pico_sdk_init.cmake)

        add_compile_definitions(OBC_PLATFORM_RP2040)
    endif()
endmacro()

macro(obc_init_sdk)
    set(OBC_HAL_SOURCE_DIR ${PROJECT_SOURCE_DIR}/platform/${OBC_PLATFORM}/hal)

    add_library(obc_hal INTERFACE)
    target_sources(obc_hal INTERFACE
        ${OBC_HAL_SOURCE_DIR}/adc_driver.c
        ${OBC_HAL_SOURCE_DIR}/board_control.c
        ${OBC_HAL_SOURCE_DIR}/flash_hal_driver.c
        ${OBC_HAL_SOURCE_DIR}/gpio_driver.c
        ${OBC_HAL_SOURCE_DIR}/i2c_driver.c
        ${OBC_HAL_SOURCE_DIR}/multicore.c
        ${OBC_HAL_SOURCE_DIR}/pwm_driver.c
        ${OBC_HAL_SOURCE_DIR}/serial_driver.c
        ${OBC_HAL_SOURCE_DIR}/spi_driver.c
        ${OBC_HAL_SOURCE_DIR}/time_tracker.c
        ${OBC_HAL_SOURCE_DIR}/uart_driver.c
    )
    target_include_directories(obc_hal INTERFACE ${PROJECT_SOURCE_DIR}/platform/include ${PROJECT_SOURCE_DIR}/platform/rp2040/boards/${OBC_BOARD})

    if(OBC_PLATFORM STREQUAL rp2040)
        pico_sdk_init()

        target_link_libraries(obc_hal INTERFACE hardware_adc hardware_i2c hardware_pwm hardware_spi hardware_uart hardware_flash hardware_sync pico_multicore pico_stdlib)
    endif()
endmacro()