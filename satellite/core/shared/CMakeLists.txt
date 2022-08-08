add_library(core_shared STATIC
    include/shared/commands_utils.h
    include/shared/hardware_utils.h
    include/shared/measurements_utils.h
    include/shared/radio_utils.h
    src/commands_utils.c
)

target_include_directories(core_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)