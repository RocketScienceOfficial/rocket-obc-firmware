add_library(core_shared
    radio_utils.h
    measurements_utils.h
    commands_utils.h
    commands_utils.c
)

target_link_libraries(core_shared measurements console commands)

target_include_directories(core_shared PUBLIC .)