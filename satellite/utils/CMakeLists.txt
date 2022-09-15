add_library(utils_lib STATIC
    include/utils/constants.h
    include/utils/cryptography.h
    include/utils/multithreading.h
    include/utils/time_tracker.h
    src/cryptography.c
    src/multithreading.c
    src/time_tracker.c
    )

target_link_libraries(utils_lib PRIVATE pico_stdlib pico_multicore)
target_include_directories(utils_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)