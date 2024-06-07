#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

// --- ADC ---
#define ADC_VREF 2.5f
#define ADC_IGN_1_DET_CH 2
#define ADC_IGN_2_DET_CH 3
#define ADC_IGN_3_DET_CH 4
#define ADC_IGN_4_DET_CH 5

// --- PINS ---
#define PIN_BATTERY 26
#define PIN_LED 21
#define PIN_IGN_1 14
#define PIN_IGN_2 13
#define PIN_IGN_3 12
#define PIN_IGN_4 11
#define PIN_CS_H3LIS 0
#define PIN_CS_LSM 1
#define PIN_CS_MMC 9
#define PIN_CS_BMI_ACC 6
#define PIN_CS_BMI_GYRO 5
#define PIN_CS_MS56 8
#define PIN_CS_NEO 7
#define PIN_CS_ADC 10

// --- SPI ---
#define OBC_SPI 0
#define OBC_SPI_FREQ 1 * 1000 * 1000
#define OBC_SPI_SCK_PIN 2
#define OBC_SPI_MOSI_PIN 3
#define OBC_SPI_MISO_PIN 4

// --- UART ---
#define OBC_UART 0
#define OBC_UART_FREQ 115200
#define OBC_UART_TX 16
#define OBC_UART_RX 17

#endif