#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

// --- ADC ---
#define ADC_VREF 2.5f

// --- PINS ---
#define PIN_LED 15
#define PIN_BUZZER 22
#define PIN_IGN_EN_1 21
#define PIN_IGN_EN_2 18
#define PIN_IGN_EN_3 20
#define PIN_IGN_EN_4 19
#define PIN_IGN_DET_1 29
#define PIN_IGN_DET_2 28
#define PIN_IGN_DET_3 27
#define PIN_IGN_DET_4 26
#define PIN_CS_H3LIS 0
#define PIN_CS_LSM 1
#define PIN_CS_MMC 9
#define PIN_CS_BMI_ACC 6
#define PIN_CS_BMI_GYRO 5
#define PIN_CS_MS56 7
#define PIN_CS_NEO 12
#define PIN_CS_ADS 13
#define PIN_VBAT 25
#define PIN_5V 23
#define PIN_3V3 24

// --- SPI ---
#define OBC_SPI 0
#define OBC_SPI_FREQ 1 * 1000 * 1000
#define OBC_SPI_SCK_PIN 2
#define OBC_SPI_MOSI_PIN 3
#define OBC_SPI_MISO_PIN 4

// --- UART ---
#define OBC_UART 0
#define OBC_UART_FREQ 1843200
#define OBC_UART_TX 16
#define OBC_UART_RX 17

// --- FLASH ---
#define FLASH_PAGE_SIZE 256
#define FLASH_SECTOR_SIZE 4096

#endif