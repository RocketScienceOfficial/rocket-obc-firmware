add_library(radio_sender_tests STATIC
    test_radio_sender.h
    test_radio_sender.c
    )

target_link_libraries(radio_sender_tests PRIVATE logging_lib utils_lib)
target_include_directories(radio_sender_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../shared)