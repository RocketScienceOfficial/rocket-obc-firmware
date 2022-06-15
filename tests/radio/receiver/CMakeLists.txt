add_library(radio_receiver_test test_radio_receiver.h test_radio_receiver.c)

target_link_libraries(radio_receiver_test PUBLIC time_tracker logging radio test_framework)

target_include_directories(radio_receiver_test PUBLIC . ../)