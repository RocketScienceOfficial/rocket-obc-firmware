link_libraries(pico_stdlib api_lib crypto_lib maths_lib ahrs_lib drivers_lib services_lib)

add_subdirectory(common)

link_libraries(core_common_lib)

add_subdirectory(ground_station)
add_subdirectory(prototyping)
add_subdirectory(rocket)