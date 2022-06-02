add_library(radio
    lora.c
    lora.h
    radioProtocol.h
    radioProtocol.c
    )

target_link_libraries(radio logging pico_stdlib hardware_spi hardware_interp myassert maths)
target_include_directories(radio PUBLIC .)