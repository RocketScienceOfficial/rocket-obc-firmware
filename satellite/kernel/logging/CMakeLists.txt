set(MODULE_NAME kernel)
set(LIB_NAME logging)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/logger.h
    src/logger.c
)

target_link_libraries(kernel_logging_lib PRIVATE kernel_console_lib drivers_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)