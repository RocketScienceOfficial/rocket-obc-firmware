cmake_minimum_required(VERSION 3.12)

include(pico-sdk/pico_sdk_init.cmake)

project(obc C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(lib)

# add_subdirectory(boot)
add_subdirectory(debug)

add_subdirectory(api)

add_subdirectory(tools)
add_subdirectory(maths)
add_subdirectory(crypto)
add_subdirectory(drivers)
add_subdirectory(kernel)
add_subdirectory(core)

add_subdirectory(tests)