link_libraries(pico_stdlib utils_lib drivers_lib kernel_lib)

add_subdirectory(shared)

add_subdirectory(dataReceiver)
add_subdirectory(measure)
add_subdirectory(prototyping)