set(MODULE_NAME kernel)
set(LIB_NAME services)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/driver_calling.h
    include/${MODULE_NAME}/${LIB_NAME}/radio_protocol.h
    src/radio_protocol.c
)

target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)