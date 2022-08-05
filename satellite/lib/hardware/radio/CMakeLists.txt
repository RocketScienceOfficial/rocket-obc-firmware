add_library(radio_lib
    radio.c
    radio.h
    radio_protocol.h
    radio_protocol.c
)

target_link_libraries(radio_lib errors_lib maths_lib pico_stdlib hardware_spi hardware_interp)
target_include_directories(radio_lib PUBLIC .)