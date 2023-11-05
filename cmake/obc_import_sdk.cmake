macro(obc_include_sdk)
    include(lib/pico-sdk/pico_sdk_init.cmake)
endmacro()

macro(obc_init_sdk)
    pico_sdk_init()
endmacro()