add_library(sd
    sd.h
    sd.c
    )

target_link_libraries(sd logging pico_stdlib FatFs_SPI time_tracker)
target_include_directories(sd PUBLIC .)