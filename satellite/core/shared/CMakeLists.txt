add_library(core_shared
    radio_utils.h
    measurements_utils.h
    commands_utils.h
    commands_utils.c
    hardware_utils.h
)

target_link_libraries(core_shared barometer_lib accelerometer_lib gps_lib console_lib commands_lib errors_lib)

target_include_directories(core_shared PUBLIC .)