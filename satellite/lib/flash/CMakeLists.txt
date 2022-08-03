add_library(flash
    flash_control.h
    flash_control.c
    )

target_link_libraries(flash logging pico_stdlib hardware_flash hardware_sync)

target_include_directories(flash PUBLIC .)