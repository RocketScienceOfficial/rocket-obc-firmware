// #pragma once

// #define RADIO_SEND_RATE_MS 500
// #define MEASUREMENTS_UPDATE_RATE_MS 2.5
// #define DIODES_UPDATE_RATE_MS 1000
// #define COMMANDS_START_TIME_MS 30000

// #define TX_PIN 0
// #define RX_PIN 1

// #define ENABLE_3V3_PIN 2
// #define ENABLE_5V_PIN 9
// #define ENABLE_VBAT_PIN 15

// #define IGNITER_1_PIN 21
// #define IGNITER_2_PIN 22
// #define IGNITER_3_PIN 23
// #define IGNITER_4_PIN 7
// #define IGNITER_1_CON_PIN 26
// #define IGNITER_2_CON_PIN 27
// #define IGNITER_3_CON_PIN 28
// #define IGNITER_4_CON_PIN 29

// #define SERVO_1_PIN 3
// #define SERVO_2_PIN 4
// #define SERVO_3_PIN 5
// #define SERVO_4_PIN 6

// #define BUZZER_PIN 24
// #define RGB_LED_PIN 8
// #define ARM_PIN 25

// #define SPI 1
// #define I2C 1
// #define SCK 10
// #define MISO 11
// #define MOSI 12
// #define EXTERNAL_CS_PIN 16
// #define SDA 14
// #define SCL 13

// #define LORA_CS_PIN 17
// #define LORA_RESET_PIN 18
// #define LORA_BUSY_PIN 19
// #define LORA_DIO1_PIN 20

#pragma once

#define BOARD_START_SLEEP_TIME_MS 0
#define RADIO_SEND_RATE_MS 500
#define MEASUREMENTS_UPDATE_RATE_MS 2.5
#define DIODES_UPDATE_RATE_MS 1000
#define COMMANDS_START_TIME_MS 30000
#define MISSION_TIME_MS 120000
#define IGNITER_DELAY_MS 1000

#define TX_PIN 0
#define RX_PIN 1

#define ENABLE_3V3_PIN 2
#define ENABLE_5V_PIN 9
#define ENABLE_VBAT_PIN 15

#define IGNITER_1_PIN 21
#define IGNITER_2_PIN 22
#define IGNITER_3_PIN 23
#define IGNITER_4_PIN 7
#define IGNITER_1_CON_PIN 26
#define IGNITER_2_CON_PIN 27
#define IGNITER_3_CON_PIN 28
#define IGNITER_4_CON_PIN 29

#define SERVO_1_PIN 3
#define SERVO_2_PIN 4
#define SERVO_3_PIN 5
#define SERVO_4_PIN 6

#define RGB_LED_PIN 15

#define SPI 0
#define SCK 18
#define MISO 16
#define MOSI 19
#define EXTERNAL_CS_PIN 16
#define BMI088_ACCEL_CS_PIN 11
#define BMI088_GYRO_CS_PIN 10
#define BME688_CS_PIN 4
#define H3LIS331DL_CS_PIN 1
#define LSM6DSO32_CS_PIN 14
#define MMC5983MA_CS_PIN 12
#define UBLOX_NEO_M9N_CS_PIN 13

#define LORA_CS_PIN 21
#define LORA_RESET_PIN 22