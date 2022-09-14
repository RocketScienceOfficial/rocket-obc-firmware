add_library(drivers_flash_lib STATIC
    include/drivers/flash/flash_driver.h
    src/flash_driver.c
    )

target_link_libraries(drivers_flash_lib PUBLIC pico_stdlib hardware_flash hardware_sync utils_lib)
target_include_directories(drivers_flash_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)