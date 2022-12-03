set(LIB_NAME ahrs)
set(LIB_GENERAL_NAME "${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${LIB_NAME}/kalman_filter.h
    include/${LIB_NAME}/madgwick_filter.h
    src/kalman_filter.c
    src/madgwick_filter.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE api_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)