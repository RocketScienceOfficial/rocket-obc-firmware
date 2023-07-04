set(LIB_GENERAL_NAME services_lib)

add_library(${LIB_GENERAL_NAME} main.c)

target_link_libraries(${LIB_GENERAL_NAME} INTERFACE
    services_logging_lib
)