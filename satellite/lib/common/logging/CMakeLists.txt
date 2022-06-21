add_library(logging
    log_serial.h
    log_serial.c
    logger.h
    logger.c
    recorder.h
    recorder.c
    )

target_link_libraries(logging time_tracker)

target_include_directories(logging PUBLIC .)