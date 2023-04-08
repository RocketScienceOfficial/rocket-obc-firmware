#!/bin/bash

cd $(dirname $0)

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

cp build/core/ground_station/ground_station.uf2 executables/core/ground_station.uf2
cp build/core/satellite/satellite.uf2 executables/core/satellite.uf2
cp build/core/prototyping/prototyping.uf2 executables/core/prototyping.uf2

cp build/tests/tests_runner/tests_runner.uf2 executables/tests/tests_runner.uf2

cp build/samples/battery/samples_battery.uf2 executables/samples/battery.uf2
cp build/samples/bme688/samples_bme688.uf2 executables/samples/bme688.uf2
cp build/samples/bmi088/samples_bmi088.uf2 executables/samples/bmi088.uf2
cp build/samples/detonator/samples_detonator.uf2 executables/samples/detonator.uf2
cp build/samples/flash/samples_flash.uf2 executables/samples/flash.uf2
cp build/samples/gpio/samples_gpio.uf2 executables/samples/gpio.uf2
cp build/samples/h3lis331dl/samples_h3lis331dl.uf2 executables/samples/h3lis331dl.uf2
cp build/samples/mmc5983ma/samples_mmc5983ma.uf2 executables/samples/mmc5983ma.uf2
cp build/samples/rp2040/samples_rp2040.uf2 executables/samples/rp2040.uf2
cp build/samples/sd/samples_sd.uf2 executables/samples/sd.uf2
cp build/samples/simple_buzzer/samples_simple_buzzer.uf2 executables/samples/simple_buzzer.uf2
cp build/samples/sx127X/samples_sx127X.uf2 executables/samples/sx127X.uf2
cp build/samples/ubx_neo_m9n/samples_ubx_neo_m9n.uf2 executables/samples/ubx_neo_m9n.uf2
cp build/samples/ws2812/samples_ws2812.uf2 executables/samples/ws2812.uf2