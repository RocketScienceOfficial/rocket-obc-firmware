set(LIB_NAME crypto)
set(LIB_GENERAL_NAME "${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${LIB_NAME}/aes_encryption.h
    include/${LIB_NAME}/parity_check.h
    include/${LIB_NAME}/rsa_encryption.h
    include/${LIB_NAME}/sha256_hash.h
    include/${LIB_NAME}/xor_encryption.h
    src/aes_encryption.c
    src/parity_check.c
    src/rsa_encryption.c
    src/sha256_hash.c
    src/xor_encryption.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE tools_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)