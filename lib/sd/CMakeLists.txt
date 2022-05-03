add_library(sd
    sd.h
    sd.c
    )

target_link_libraries(sd logging misc pico_stdlib FatFs_SPI)
target_include_directories(sd PUBLIC .)