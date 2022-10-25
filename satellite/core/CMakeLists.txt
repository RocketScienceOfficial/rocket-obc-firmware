link_libraries(pico_stdlib tools_lib drivers_lib kernel_lib)

add_subdirectory(shared)

add_subdirectory(ground_station)
add_subdirectory(prototyping)
add_subdirectory(satellite)