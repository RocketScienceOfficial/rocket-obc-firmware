add_library(flash_lib
    flash_control.h
    flash_control.c
    )

target_link_libraries(flash_lib errors_lib pico_stdlib hardware_flash hardware_sync)

target_include_directories(flash_lib PUBLIC .)