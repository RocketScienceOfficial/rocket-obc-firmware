cmake_minimum_required(VERSION 3.12)

set(OBC_PLATFORM rp2040 CACHE STRING "OBC Platform")
message("OBC_PLATFORM is ${OBC_PLATFORM}")

set(OBC_BOARD obcv15 CACHE STRING "OBC Board")
message("OBC_BOARD is ${OBC_BOARD}")

include(cmake/obc_import_sdk.cmake)
include(cmake/obc_lib.cmake)
include(cmake/obc_outputs.cmake)

obc_include_sdk()

project(rocket_obc C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

obc_init_sdk()

add_subdirectory(lib)
add_subdirectory(src)