add_library(test_framework INTERFACE)

target_link_libraries(test_framework INTERFACE module_drivers_hal)

target_include_directories(test_framework INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)