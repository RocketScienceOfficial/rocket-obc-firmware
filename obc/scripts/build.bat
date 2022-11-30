@echo off

pushd %~dp0\..\obc

if not exist build mkdir build

cd build

call cmake -G "Ninja" ..
call ninja

cd ..

if not exist executables mkdir executables

copy build\core\ground_station\ground_station.uf2 executables\ground_station.uf2
copy build\core\satellite\satellite.uf2 executables\satellite.uf2
copy build\core\prototyping\prototyping.uf2 executables\prototyping.uf2
copy build\tests\tests_runner\tests_runner.uf2 executables\tests_runner.uf2
copy build\debug\picoprobe\picoprobe.uf2 executables\picoprobe.uf2

popd