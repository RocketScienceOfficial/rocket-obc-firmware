#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "pico/stdlib.h"
#include "pinout_config.h"
#include "logger.h"
#include "time_tracker.h"
#include "console_input.h"
#include "console_colors.h"
#include "console_printer.h"
#include "log_printer.h"
#include "commands_printer.h"
#include "console_commands.h"
#include "default_console_commands.h"
#include "my_assert.h"
#include "sd.h"
#include "lora.h"
#include "bmp280.h"
#include "mpu6050.h"
#include "kalman_filter.h"

static lora_data_t s_LoraData;
static kalman_filter_data_t s_KalmanData;
static double s_VelocityX;
static double s_VelocityY;
static double s_VelocityZ;
static unsigned int s_TimerOffset = 0;

void start();
void initialize();
void loop();
void checkCommand();

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
    resetColorsAndEffects();

    attachPrinterToConsoleLog();
    consoleSetUser("measure");
    consoleStart();

    consoleInputAttachToLogger(NULL, 0);
    attachPrinterToLog();
    attachPrinterToCommandsLog();
    consoleInputAttachToPrinter(NULL, 0);

    MY_LOG_CORE_INFO("Initializing...");

    resetColorsAndEffects();

    registerDefaultConsoleCommands();

/*    lora_pinout_t loraPinout = {
        .spi = SX1278_SPI,
        .sck = SX1278_SCK_GPIO,
        .miso = SX1278_MISO_GPIO,
        .mosi = SX1278_MOSI_GPIO,
        .cs = SX1278_CS_GPIO,
        .ss = SX1278_SS_GPIO,
        .reset = SX1278_RESET_GPIO,
        .dio0 = SX1278_DIO0_GPIO};

    loraInit(&s_LoraData, &loraPinout);

    s_LoraData.pinout = loraPinout;
    
    MY_ASSERT(loraBegin(&s_LoraData, SX1278_FREQ_HZ));

    sdInit();
    sdInitFile(LOG_CORE_FILENAME);
    sdFlush(LOG_CORE_FILENAME);
    sdInitFile(LOG_MEASUREMENTS_FILENAME);
    sdFlush(LOG_MEASUREMENTS_FILENAME);
    sdAttachToCoreLogger();
    sdAttachToMeasureLogger();

    sdBegin(LOG_CORE_FILENAME);
*/
  //  bmp280Init(BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN);
    mpu6050Init(MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN);

    kalmanInit(&s_KalmanData);

    MY_LOG_CORE_INFO("Everything is ready!");

    //sdEnd(LOG_CORE_FILENAME);
}

void loop()
{
    checkCommand();

    if (runEvery(500, &s_TimerOffset))
    {
       // sdBegin(LOG_CORE_FILENAME);
       // sdBegin(LOG_MEASUREMENTS_FILENAME);

       // bmp280_data_t bmp280Data;
       // bmp280Read(&bmp280Data);

       // MY_LOG_MEASURE("BMP280 Pressure: %f Pa", bmp280Data.pressure);
       // MY_LOG_MEASURE("BMP280 Temperature: %f C", bmp280Data.temperature);

       // MY_LOG_MEASURE_END();

        mpu6050_data_t mpu6050Data;
        mpu6050ReadRaw(&mpu6050Data);
        mpu6050ConvertData(&mpu6050Data);

        double accel_x = mpu6050Data.accel_x;//kalman(&s_KalmanData, mpu6050Data.accel_x);
        double accel_y = mpu6050Data.accel_y;//kalman(&s_KalmanData, mpu6050Data.accel_y);
        double accel_z = mpu6050Data.accel_z;//kalman(&s_KalmanData, mpu6050Data.accel_z);
        double rot_x = mpu6050Data.rot_x;//kalman(&s_KalmanData, mpu6050Data.rot_x);
        double rot_y = mpu6050Data.rot_y;//kalman(&s_KalmanData, mpu6050Data.rot_y);
        double rot_z = mpu6050Data.rot_z;//kalman(&s_KalmanData, mpu6050Data.rot_z);

        MY_LOG_CORE_INFO("AccelX: %f", accel_x);
        MY_LOG_CORE_INFO("AccelY: %f", accel_y);
        MY_LOG_CORE_INFO("AccelZ: %f", accel_z);
/*        MY_LOG_CORE_INFO("RotX: %f", rot_x);
        MY_LOG_CORE_INFO("RotY: %f", rot_y);
        MY_LOG_CORE_INFO("RotZ: %f", rot_z);
*/
        s_VelocityX = s_VelocityX + accel_x * 0.5;
        s_VelocityY = s_VelocityY + accel_y * 0.5;
        s_VelocityZ = s_VelocityZ + accel_z * 0.5;
/*
        MY_LOG_CORE_INFO("VelX: %f", s_VelocityX*3.6);
        MY_LOG_CORE_INFO("VelY: %f", s_VelocityY*3.6);
        MY_LOG_CORE_INFO("VelZ: %f", s_VelocityZ*3.6);
*/
        // myLogMeasureBegin();
        // myLogMeasure("%d", bmp280Data.temperature);
        // myLogMeasure("%d", bmp280Data.pressure);
        // myLogMeasure("%f", mpu6050Data.temperature);
        // myLogMeasure("%f", mpu6050Data.accel_x);
        // myLogMeasure("%f", mpu6050Data.accel_y);
        // myLogMeasure("%f", mpu6050Data.accel_z);
        // myLogMeasure("%f", mpu6050Data.rot_x);
        // myLogMeasure("%f", mpu6050Data.rot_y);
        // myLogMeasure("%f", mpu6050Data.rot_z);
        // myLogMeasureEnd();
/*
        MY_LOG_CORE_INFO("Sending packet...");

        loraBeginPacket(&s_LoraData, 0);
        loraWrite_str(&s_LoraData, "Hello World!");
        loraEndPacket(&s_LoraData, 0);

        sdEnd(LOG_CORE_FILENAME);
        sdEnd(LOG_MEASUREMENTS_FILENAME);
        */
    }

    // int packetSize = loraParsePacket(&s_LoraData, 0);

    // if (packetSize)
    // {
    //     MY_LOG_CORE_INFO("Received packet '");

    //     while (loraAvailable(&s_LoraData))
    //     {
    //         MY_LOG_CORE_INFO("%c", (char)loraRead(&s_LoraData));
    //     }
    // }
}

void checkCommand()
{
    char **tokens;
    size_t tokensSize = 0;
    consoleCheckInput(&tokens, &tokensSize);

    if (tokensSize > 0)
    {
        char **commandArgs;
        size_t commandArgsSize = 0;
        console_command_t *command = parseCommand(tokens, tokensSize, &commandArgs, &commandArgsSize);

        if (command != NULL)
        {
            executeCommand(command, commandArgs, commandArgsSize);
        }

        if (commandArgsSize > 0)
        {
            free(commandArgs);
        }

        for (int i = 0; i < tokensSize; i++)
        {
            free(tokens[i]);
        }

        free(tokens);
    }
}
