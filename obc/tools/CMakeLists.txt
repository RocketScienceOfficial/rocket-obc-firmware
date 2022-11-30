set(LIB_NAME tools)
set(LIB_GENERAL_NAME "${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${LIB_NAME}/multithreading.h
    include/${LIB_NAME}/time_tracker.h
    include/${LIB_NAME}/typedefs.h
    src/multithreading.c
    src/time_tracker.c
)

target_link_libraries(${LIB_GENERAL_NAME} PUBLIC pico_stdlib pico_multicore)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)