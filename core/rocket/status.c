#include "status.h"
#include "config.h"
#include "drivers/led/w2812_driver.h"
#include "drivers/adc/max1161X_driver.h"
#include "drivers/gpio/adc_driver.h"
#include "drivers/tools/time_tracker.h"
#include "drivers/battery/battery_driver.h"

#define LEDS_COUNT 7
#define LORA_TX_TIME_MS 50
#define BATTERY_INTERVALS_COUNT 10

static WS2812COLOR s_Colors[LEDS_COUNT];
static MAX1161XConfig s_ADCConfig;
static BatteryInterval s_BatteryIntervals[BATTERY_INTERVALS_COUNT];
static TIME s_DiodesTimerOffset;
static TIME s_LoRaTXStartTime;
static BOOL s_LoRaTXState;

static VOID __refreshColors(VOID);
static WS2812COLOR __getDetonatorConColor(VoltageLevel voltage);
static WS2812COLOR __getARMColor(BOOL state);
static WS2812COLOR __getBatteryColor(FLOAT percent);
static WS2812COLOR __getLoRaTXColor(BOOL state);

VOID initStatus(VOID)
{
    ws2812Init(RGB_LED_PIN, FALSE);
    max1161XInit(&s_ADCConfig, MAX1161X_TYPE_3, I2C, SDA, SCL);
    gpioInitPin(ARM_PIN, GPIO_OUTPUT);

    adcInitPin(0);
    adcInitPin(1);
    adcInitPin(2);
    adcInitPin(3);

    s_BatteryIntervals[0] = (BatteryInterval){0.7273f, 0.7636f, 90.0f, 100.0f};
    s_BatteryIntervals[1] = (BatteryInterval){0.7182, 0.7273f, 80.0f, 90.0f};
    s_BatteryIntervals[2] = (BatteryInterval){0.6999f, 0.7182f, 70.0f, 80.0f};
    s_BatteryIntervals[3] = (BatteryInterval){0.6909f, 0.6999f, 60.0f, 70.0f};
    s_BatteryIntervals[4] = (BatteryInterval){0.6818f, 0.6909f, 50.0f, 60.0f};
    s_BatteryIntervals[5] = (BatteryInterval){0.6764, 0.6818f, 40.0f, 50.0f};
    s_BatteryIntervals[6] = (BatteryInterval){0.6727f, 0.6764f, 30.0f, 40.0f};
    s_BatteryIntervals[7] = (BatteryInterval){0.6673, 0.6727f, 20.0f, 30.0f};
    s_BatteryIntervals[8] = (BatteryInterval){0.6636f, 0.6673f, 10.0f, 20.0f};
    s_BatteryIntervals[9] = (BatteryInterval){0.5455f, 0.6636f, 0.0f, 10.0f};
}

VOID updateStatus(VOID)
{
    if (runEveryUs(DIODES_UPDATE_RATE_MS * 1000, &s_DiodesTimerOffset))
    {
        VoltageLevel det1, det2, det3, det4, currentConsumption, batteryVoltage, usbConnection;
        GPIOState armState;

        adcRead(0, &det1);
        adcRead(1, &det2);
        adcRead(2, &det3);
        adcRead(3, &det4);

        gpioGetPinState(ARM_PIN, &armState);

        max1161XRead(&s_ADCConfig, MAX1161X_CHANNEL_AIN0, &currentConsumption);
        max1161XRead(&s_ADCConfig, MAX1161X_CHANNEL_AIN1, &batteryVoltage);
        max1161XRead(&s_ADCConfig, MAX1161X_CHANNEL_AIN2, &usbConnection);

        FLOAT batteryPercentage = batteryConvertVoltageToPercent(s_BatteryIntervals, BATTERY_INTERVALS_COUNT, batteryVoltage);

        s_Colors[0] = __getDetonatorConColor(det1);
        s_Colors[1] = __getDetonatorConColor(det2);
        s_Colors[2] = __getDetonatorConColor(det3);
        s_Colors[3] = __getDetonatorConColor(det4);
        s_Colors[4] = __getARMColor(armState == GPIO_HIGH);
        s_Colors[5] = __getBatteryColor(batteryPercentage);

        __refreshColors();
    }

    if (s_LoRaTXState)
    {
        if (getMsSinceBoot() - s_LoRaTXStartTime >= LORA_TX_TIME_MS)
        {
            setLoRaTXDiode(FALSE);
        }
    }
}

VOID setLoRaTXDiode(BOOL state)
{
    s_Colors[6] = __getLoRaTXColor(state);

    s_LoRaTXState = state;

    if (state)
    {
        s_LoRaTXStartTime = getMsSinceBoot();
    }

    __refreshColors();
}

static VOID __refreshColors(VOID)
{
    ws2812SetColors(s_Colors, LEDS_COUNT);
}

static WS2812COLOR __getDetonatorConColor(VoltageLevel voltage)
{
    return voltage > 0.5 ? ws2812GetColor(0, 255, 0) : ws2812GetColor(255, 0, 0);
}

static WS2812COLOR __getARMColor(BOOL state)
{
    return state ? ws2812GetColor(0, 255, 0) : ws2812GetColor(255, 0, 0);
}

static WS2812COLOR __getBatteryColor(FLOAT percent)
{
    if (percent >= 50)
    {
        return ws2812GetColor(255 - 255 * (percent - 50) / 50, 255, 0);
    }
    else
    {
        return ws2812GetColor(255, 255 * percent / 50, 0);
    }
}

static WS2812COLOR __getLoRaTXColor(BOOL state)
{
    return state ? ws2812GetColor(255, 0, 0) : ws2812GetColor(0, 0, 0);
}