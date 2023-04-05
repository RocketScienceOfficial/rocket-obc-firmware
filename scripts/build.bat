@echo off

pushd %~dp0\..

if not exist build mkdir build

cd build

call cmake -G "Ninja" ..
call ninja

cd ..

if not exist executables mkdir executables
if not exist executables\core mkdir executables\core
if not exist executables\tests mkdir executables\tests
if not exist executables\samples mkdir executables\samples

copy build\core\ground_station\ground_station.uf2 executables\core\ground_station.uf2
copy build\core\satellite\satellite.uf2 executables\core\satellite.uf2
copy build\core\prototyping\prototyping.uf2 executables\core\prototyping.uf2

copy build\tests\tests_runner\tests_runner.uf2 executables\tests\tests_runner.uf2

copy build\samples\battery\samples_battery.uf2 executables\samples\battery.uf2
copy build\samples\bme688\samples_bme688.uf2 executables\samples\bme688.uf2
copy build\samples\bmi088\samples_bmi088.uf2 executables\samples\bmi088.uf2
copy build\samples\detonator\samples_detonator.uf2 executables\samples\detonator.uf2
copy build\samples\flash\samples_flash.uf2 executables\samples\flash.uf2
copy build\samples\gpio\samples_gpio.uf2 executables\samples\gpio.uf2
copy build\samples\h3lis331dl\samples_h3lis331dl.uf2 executables\samples\h3lis331dl.uf2
copy build\samples\mmc5983ma\samples_mmc5983ma.uf2 executables\samples\mmc5983ma.uf2
copy build\samples\rp2040\samples_rp2040.uf2 executables\samples\rp2040.uf2
copy build\samples\sd\samples_sd.uf2 executables\samples\sd.uf2
copy build\samples\simple_buzzer\samples_simple_buzzer.uf2 executables\samples\simple_buzzer.uf2
copy build\samples\sx127X\samples_sx127X.uf2 executables\samples\sx127X.uf2
copy build\samples\ubx_neo_m9n\samples_ubx_neo_m9n.uf2 executables\samples\ubx_neo_m9n.uf2
copy build\samples\ws2812\samples_ws2812.uf2 executables\samples\ws2812.uf2

popd