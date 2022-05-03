add_library(radio
    lora.c
    lora.h
    )

target_link_libraries(radio logging pico_stdlib hardware_spi hardware_interp)
target_include_directories(radio PUBLIC .)