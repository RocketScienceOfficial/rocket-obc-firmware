link_libraries(pico_stdlib api_lib tools_lib crypto_lib maths_lib ahrs_lib drivers_lib kernel_lib)

add_subdirectory(common)

add_subdirectory(ground_station)
add_subdirectory(prototyping)
add_subdirectory(satellite)