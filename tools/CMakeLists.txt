set(LIB_NAME tools)
set(LIB_GENERAL_NAME "${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${LIB_NAME}/board_control.h
    include/${LIB_NAME}/multithreading.h
    include/${LIB_NAME}/time_tracker.h
    src/board_control.c
    src/multithreading.c
    src/time_tracker.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE pico_multicore)
target_link_libraries(${LIB_GENERAL_NAME} PUBLIC api_lib pico_stdlib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)