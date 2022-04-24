add_library(logging
    logger.h
    logger.c
    measurements_logger.h
    measurements_logger.c
    )

target_link_libraries(logging misc)

target_include_directories(logging PUBLIC .)