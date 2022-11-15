set(MODULE_NAME tests_kernel)
set(LIB_NAME services)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}")

add_library(${LIB_GENERAL_NAME} STATIC
    test_radio_protocol.h
    test_radio_protocol.c
    )

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE kernel_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})