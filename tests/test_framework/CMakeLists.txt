add_library(test_framework INTERFACE)

target_link_libraries(test_framework INTERFACE obc_hal)

target_include_directories(test_framework INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)