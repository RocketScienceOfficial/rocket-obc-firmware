add_library(radio_protocol_tests test_radio_protocol.h test_radio_protocol.c)

target_link_libraries(radio_protocol_tests PUBLIC radio_lib test_framework)

target_include_directories(radio_protocol_tests PUBLIC .)