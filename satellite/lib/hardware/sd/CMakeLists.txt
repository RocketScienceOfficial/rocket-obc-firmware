add_library(sd_lib
    sd.h
    sd.c
    )

target_link_libraries(sd_lib errors_lib pico_stdlib FatFs_SPI)
target_include_directories(sd_lib PUBLIC .)