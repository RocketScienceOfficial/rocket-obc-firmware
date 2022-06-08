#include "pico/stdlib.h"
#include "pinout.h"
#include "mpu6050.h"
#include "test_framework.h"
#include "logger.h"
#include "log_serial.h"
#include "time_tracker.h"

static unsigned int s_TimerOffset = 0;

void start();
void initialize();
void loop();

int main()
{
    start();

    initialize();

    while (1)
    {
        loop();
    }

    return 0;
}

void start()
{
    stdio_init_all();
    sleep_ms(5000);
}

void initialize()
{
    attachSerialToLog(myLogGetCoreLogger());

    MY_LOG_CORE_INFO("Initializing...");

    mpu6050Init(MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN);

    MY_LOG_CORE_INFO("Everything is ready!");
}

void loop()
{
    if (runEvery(1000, &s_TimerOffset))
    {
        mpu6050_data_t mpu6050Data;
        mpu6050ReadRaw(&mpu6050Data);

        double accel_x = mpu6050Data.accel_x;
        double accel_y = mpu6050Data.accel_y;
        double accel_z = mpu6050Data.accel_z;
        double rot_x = mpu6050Data.rot_x;
        double rot_y = mpu6050Data.rot_y;
        double rot_z = mpu6050Data.rot_z;

        MY_LOG_CORE_INFO("AccelX: %f", accel_x);
        MY_LOG_CORE_INFO("AccelY: %f", accel_y);
        MY_LOG_CORE_INFO("AccelZ: %f", accel_z);
        MY_LOG_CORE_INFO("RotX: %f", rot_x);
        MY_LOG_CORE_INFO("RotY: %f", rot_y);
        MY_LOG_CORE_INFO("RotZ: %f", rot_z);
    }
}