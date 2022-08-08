@echo off

pushd %~dp0\..\satellite

if not exist build mkdir build

cd build

call cmake -G "Ninja" ..
call ninja

cd ..

if not exist executables mkdir executables

copy build\core\dataReceiver\dataReceiver.uf2 executables\dataReceiver.uf2
copy build\core\measure\measure.uf2 executables\measure.uf2
copy build\tests\_runners\dynamic\dynamic_tests_runner.uf2 executables\dynamic_tests_runner.uf2
copy build\tests\_runners\static\static_tests_runner.uf2 executables\static_tests_runner.uf2

popd