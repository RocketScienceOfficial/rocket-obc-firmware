#!/bin/bash

cd $(dirname $0)/..

if [ ! -d build ]; then
   mkdir build
fi

cd build

cmake -G "Ninja" ..
ninja

cd ..

if [ ! -d executables ]; then
   mkdir executables
fi

if [ ! -d executables/core ]; then
   mkdir executables/core
fi

if [ ! -d executables/tests ]; then
   mkdir executables/tests
fi

if [ ! -d executables/samples ]; then
   mkdir executables/samples
fi

cp build/core/rocket/rocket.uf2 executables/core/rocket.uf2
cp build/core/prototyping/prototyping.uf2 executables/core/prototyping.uf2

cp build/tests/tests_runner/tests_runner.uf2 executables/tests/tests_runner.uf2

cp build/samples/battery/samples_battery.uf2 executables/samples/battery.uf2
cp build/samples/bme688/samples_bme688.uf2 executables/samples/bme688.uf2
cp build/samples/bmi088/samples_bmi088.uf2 executables/samples/bmi088.uf2
cp build/samples/bmp581/samples_bmp581.uf2 executables/samples/bmp581.uf2
cp build/samples/buzzer_active/samples_buzzer_active.uf2 executables/samples/buzzer_active.uf2
cp build/samples/buzzer_passive/samples_buzzer_passive.uf2 executables/samples/buzzer_passive.uf2
cp build/samples/cpu/samples_cpu.uf2 executables/samples/cpu.uf2
cp build/samples/igniter/samples_igniter.uf2 executables/samples/igniter.uf2
cp build/samples/flash/samples_flash.uf2 executables/samples/flash.uf2
cp build/samples/gpio/samples_gpio.uf2 executables/samples/gpio.uf2
cp build/samples/h3lis331dl/samples_h3lis331dl.uf2 executables/samples/h3lis331dl.uf2
cp build/samples/lsm6dso32/samples_lsm6dso32.uf2 executables/samples/lsm6dso32.uf2
cp build/samples/mmc5983ma/samples_mmc5983ma.uf2 executables/samples/mmc5983ma.uf2
cp build/samples/sd/samples_sd.uf2 executables/samples/sd.uf2
cp build/samples/sx126X/samples_sx126X.uf2 executables/samples/sx126X.uf2
cp build/samples/sx127X/samples_sx127X.uf2 executables/samples/sx127X.uf2
cp build/samples/ubx_neo_m9n/samples_ubx_neo_m9n.uf2 executables/samples/ubx_neo_m9n.uf2
cp build/samples/ubx_sam_m10q/samples_ubx_sam_m10q.uf2 executables/samples/ubx_sam_m10q.uf2
cp build/samples/ws2812/ws2812_reset/samples_ws2812_reset.uf2 executables/samples/ws2812_reset.uf2
cp build/samples/ws2812/ws2812_static/samples_ws2812_static.uf2 executables/samples/ws2812_static.uf2