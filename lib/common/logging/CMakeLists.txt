add_library(logging
    log_printer.h
    log_printer.c
    logger.h
    logger.c
    logging_utils.h
    logging_utils.c
    )

target_link_libraries(logging time_tracker console_colors)

target_include_directories(logging PUBLIC .)