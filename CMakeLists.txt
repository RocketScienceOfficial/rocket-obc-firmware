cmake_minimum_required(VERSION 3.12)

include(cmake/obc_import_sdk.cmake)
include(cmake/obc_add_module.cmake)
include(cmake/obc_add_platform_output.cmake)

obc_include_sdk()

project(rocket_obc)

obc_init_sdk()

#add_subdirectory(lib)
#add_subdirectory(modules)
#add_subdirectory(core)