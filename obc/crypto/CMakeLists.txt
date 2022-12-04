set(LIB_NAME crypto)
set(LIB_GENERAL_NAME "${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${LIB_NAME}/parity_check.h
    include/${LIB_NAME}/xor_encryption.h
    src/parity_check.c
    src/xor_encryption.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE api_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)