set(MODULE_NAME drivers)
set(LIB_NAME buzzer)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/active_buzzer_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/passive_buzzer_driver.h
    src/active_buzzer_driver.c
    src/passive_buzzer_driver.c
)

target_link_libraries(${LIB_GENERAL_NAME} PUBLIC drivers_gpio_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)