add_library(logging
    logger.h
    logger.c
    measurements_logger.h
    measurements_logger.c
    logging_utils.h
    logging_utils.c
    log_printer.h
    log_printer.c
    )

target_link_libraries(logging misc)

target_include_directories(logging PUBLIC .)