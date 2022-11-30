set(MODULE_NAME tests)
set(LIB_NAME crypto)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}")

add_library(${LIB_GENERAL_NAME} STATIC
    test_cryptography.h
    test_cryptography.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE crypto_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})