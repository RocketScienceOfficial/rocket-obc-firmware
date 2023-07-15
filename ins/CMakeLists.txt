set(LIB_NAME ins)
set(LIB_GENERAL_NAME "${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${LIB_NAME}/ins_kf.h
    src/ins_kf.c
)
target_link_libraries(${LIB_GENERAL_NAME} PRIVATE api_lib maths_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)