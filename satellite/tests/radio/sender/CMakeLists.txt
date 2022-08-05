add_library(radio_sender_tests test_radio_sender.h test_radio_sender.c)

target_link_libraries(radio_sender_tests PUBLIC time_tracker_lib logging_lib radio_lib test_framework)

target_include_directories(radio_sender_tests PUBLIC . ../)