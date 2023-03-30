set(LIB_GENERAL_NAME test_framework)

add_library(${LIB_GENERAL_NAME} INTERFACE
    test_framework.h
)

target_link_libraries(${LIB_GENERAL_NAME} INTERFACE tools_lib kernel_lib)
target_include_directories(${LIB_GENERAL_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})