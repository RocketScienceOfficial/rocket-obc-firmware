set(LIB_NAME obc)
set(LIB_GENERAL_NAME api_lib)

add_library(${LIB_GENERAL_NAME} INTERFACE
    include/${LIB_NAME}/api.h
)

target_include_directories(${LIB_GENERAL_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)