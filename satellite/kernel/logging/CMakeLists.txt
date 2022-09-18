add_library(kernel_logging_lib STATIC
    include/kernel/logging/logger.h
    include/kernel/logging/recorder.h
    src/logger.c
    src/recorder.c
    )

target_link_libraries(kernel_logging_lib PUBLIC utils_lib kernel_console_lib drivers_lib)
target_include_directories(kernel_logging_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)