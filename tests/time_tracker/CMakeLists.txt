add_executable(timeTrackerTest main.c)

target_link_libraries(timeTrackerTest PUBLIC pico_stdlib time_tracker logging myassert)

pico_enable_stdio_usb(timeTrackerTest 1)
pico_enable_stdio_uart(timeTrackerTest 0)

pico_add_extra_outputs(timeTrackerTest)