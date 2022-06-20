add_library(mpu6050_tests test_mpu6050.h test_mpu6050.c)

target_link_libraries(mpu6050_tests PUBLIC measurements time_tracker test_framework)

target_include_directories(mpu6050_tests PUBLIC .)