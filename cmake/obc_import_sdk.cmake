macro(obc_include_sdk)
    if(OBC_PLATFORM STREQUAL pico)
        set(PICO_BOARD_HEADER_DIRS ${CMAKE_SOURCE_DIR}/platform/pico)
        set(PICO_BOARD myboard)

        include(lib/pico-sdk/pico_sdk_init.cmake)

        add_compile_definitions(OBC_PLATFORM_PICO)
    endif()
endmacro()

macro(obc_init_sdk)
    if(OBC_PLATFORM STREQUAL pico)
        pico_sdk_init()
    endif()
endmacro()