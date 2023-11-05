set(MODULE_NAME drivers)
set(LIB_NAME gps)
set(LIB_GENERAL_NAME "${MODULE_NAME}_${LIB_NAME}_lib")

add_library(${LIB_GENERAL_NAME} STATIC
include/${MODULE_NAME}/${LIB_NAME}/nmea_parser.h
    include/${MODULE_NAME}/${LIB_NAME}/u_blox_neo_m9n_driver.h
    include/${MODULE_NAME}/${LIB_NAME}/u_blox_sam_m10q_driver.h
    src/nmea_parser.c
    src/u_blox_neo_m9n_driver.c
    src/u_blox_sam_m10q_driver.c
)

target_link_libraries(${LIB_GENERAL_NAME} PUBLIC drivers_gpio_lib)
target_include_directories(${LIB_GENERAL_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)