add_library(logging
    logger.h
    logger.c
    measurements_logger.h
    measurements_logger.c
    logging_utils.h
    logging_utils.c
    )

target_link_libraries(logging time_tracker events)

target_include_directories(logging PUBLIC .)