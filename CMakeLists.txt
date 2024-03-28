cmake_minimum_required(VERSION 3.12)

set(OBC_PLATFORM pico CACHE STRING "OBC Platform")
message("OBC_PLATFORM is ${OBC_PLATFORM}")

include(cmake/obc_import_sdk.cmake)
include(cmake/obc_modules.cmake)
include(cmake/obc_outputs.cmake)

obc_include_sdk()

project(rocket_obc C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

obc_init_sdk()

add_subdirectory(lib)
add_subdirectory(modules)
add_subdirectory(core)
add_subdirectory(tests)