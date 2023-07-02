set(MODULE_NAME drivers)
set(LIB_NAME lora)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/e19_433m20sc_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/radio_protocol.h
    include/${MODULE_NAME}/${LIB_NAME}/sx127X_driver.h
    src/e19_433m20sc_driver.c
    src/radio_protocol.c
    src/sx127X_driver.c
)

target_link_libraries(${LIB_GENERAL_NAME} PRIVATE crypto_lib)
target_link_libraries(${LIB_GENERAL_NAME} PUBLIC drivers_gpio_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)