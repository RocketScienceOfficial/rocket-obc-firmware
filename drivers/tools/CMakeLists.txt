set(MODULE_NAME drivers)
set(LIB_NAME tools)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/board_control.h
    include/${MODULE_NAME}/${LIB_NAME}/multithreading.h
    include/${MODULE_NAME}/${LIB_NAME}/time_tracker.h
    src/board_control.c
    src/multithreading.c
    src/time_tracker.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE pico_multicore)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)