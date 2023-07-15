set(LIB_NAME acs)
set(LIB_GENERAL_NAME "${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${LIB_NAME}/pid_controller.h
    src/pid_controller.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE api_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)