add_library(radio
    lora.c
    lora.h
    radio_protocol.h
    radio_protocol.c
    )

target_link_libraries(radio logging pico_stdlib hardware_spi hardware_interp maths)
target_include_directories(radio PUBLIC .)