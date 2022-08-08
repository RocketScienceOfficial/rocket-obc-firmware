add_library(radio_receiver_test STATIC
    test_radio_receiver.h
    test_radio_receiver.c
    )

target_link_libraries(radio_receiver_test PRIVATE logging_lib utils_lib)
target_include_directories(radio_receiver_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../shared)