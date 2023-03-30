set(LIB_NAME maths)
set(LIB_GENERAL_NAME "${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${LIB_NAME}/avg_filter.h
    include/${LIB_NAME}/constants.h
    include/${LIB_NAME}/exp_filter.h
    include/${LIB_NAME}/kalman_filter.h
    include/${LIB_NAME}/math_utils.h
    include/${LIB_NAME}/pid_controller.h
    include/${LIB_NAME}/quaternion.h
    include/${LIB_NAME}/vector.h
    src/avg_filter.c
    src/exp_filter.c
    src/kalman_filter.c
    src/math_utils.c
    src/pid_controller.c
    src/quaternion.c
    src/vector.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE api_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)