cmake_minimum_required(VERSION 3.12)

include(vendor/pico-sdk/pico_sdk_init.cmake)
include(cmake/utils.cmake)

project(obc C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

set(PLATFORM=PICO)

add_compile_definitions(OBC_DEBUG_MODE=1)

add_subdirectory(lib)
add_subdirectory(api)
add_subdirectory(maths)
add_subdirectory(filters)
add_subdirectory(ahrs)
add_subdirectory(crypto)
add_subdirectory(drivers)
add_subdirectory(services)
add_subdirectory(core)
add_subdirectory(tests)
add_subdirectory(samples)