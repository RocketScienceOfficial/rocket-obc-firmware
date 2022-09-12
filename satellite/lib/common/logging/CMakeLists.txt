add_library(logging_lib STATIC
    include/logging/log_serial.h
    include/logging/logger.h
    include/logging/recorder.h
    src/log_serial.c
    src/logger.c
    src/recorder.c
    )

target_link_libraries(logging_lib PUBLIC utils_lib hardware_lib)
target_include_directories(logging_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)