set(MODULE_NAME services)
set(LIB_NAME commands)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/commands.h
    include/${MODULE_NAME}/${LIB_NAME}/default_commands.h
    src/commands.c
    src/default_commands.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE services_logging_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)