set(MODULE_NAME drivers)
set(LIB_NAME console)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/console_input.h
    include/${MODULE_NAME}/${LIB_NAME}/console_output.h
    src/console_input.c
    src/console_output.c
)

target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)