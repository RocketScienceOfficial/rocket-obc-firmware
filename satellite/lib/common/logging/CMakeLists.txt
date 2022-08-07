add_library(logging_lib
    log_serial.h
    log_serial.c
    logger.h
    logger.c
    recorder.h
    recorder.c
    )

target_link_libraries(logging_lib time_tracker_lib flash_lib)

target_include_directories(logging_lib PUBLIC .)