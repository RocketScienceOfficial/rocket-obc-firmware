add_library(radio_protocol_tests STATIC
    test_radio_protocol.h
    test_radio_protocol.c
    )

target_link_libraries(radio_protocol_tests PRIVATE utils_lib)
target_include_directories(radio_protocol_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})