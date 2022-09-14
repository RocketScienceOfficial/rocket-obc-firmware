add_library(drivers_lora_lib STATIC
    include/drivers/lora/sx1278_driver.h
    src/sx1278_driver.c
    )

target_link_libraries(drivers_lora_lib PUBLIC pico_stdlib hardware_spi utils_lib)
target_include_directories(drivers_lora_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)