set(MODULE_NAME core)
set(LIB_NAME common)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/driver_calling.h
    include/${MODULE_NAME}/${LIB_NAME}/measurements_utils.h
    include/${MODULE_NAME}/${LIB_NAME}/radio_utils.h
    src/measurements_utils.c
    src/radio_utils.c
)

target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)