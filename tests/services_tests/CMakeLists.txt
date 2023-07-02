set(MODULE_NAME tests)
set(LIB_NAME services)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}")

add_library(${LIB_GENERAL_NAME} STATIC
    test_commands.h
    test_commands.c
    test_logger.h
    test_logger.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE services_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})