set(MODULE_NAME drivers)
set(LIB_NAME accelerometer)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
    include/${MODULE_NAME}/${LIB_NAME}/bmi088_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/h3lis331dl_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/lsm6dso32_driver.h
    src/bmi088_driver.c
    src/h3lis331dl_driver.c
    src/lsm6dso32_driver.c
)

target_link_libraries(${LIB_GENERAL_NAME} PUBLIC drivers_gpio_lib maths_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)