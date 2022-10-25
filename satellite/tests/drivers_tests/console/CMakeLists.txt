set(MODULE_NAME tests_drivers)
set(LIB_NAME console)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}")

add_library(${LIB_GENERAL_NAME} STATIC
    test_console.h
    test_console.c
    )

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE drivers_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})