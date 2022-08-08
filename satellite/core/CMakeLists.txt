link_libraries(pico_stdlib logging_lib console_lib commands_lib utils_lib hardware_lib)

add_subdirectory(shared)
add_subdirectory(measure)
add_subdirectory(dataReceiver)