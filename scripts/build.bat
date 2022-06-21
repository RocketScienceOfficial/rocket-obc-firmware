@echo off

pushd %~dp0\..\satellite

if not exist build mkdir build

cd build

call cmake -G "MinGW Makefiles" ..
call make -j4

cd ..

if not exist executables mkdir executables

copy build\core\dataReceiver\dataReceiver.uf2 executables\dataReceiver.uf2
copy build\core\measure\measure.uf2 executables\measure.uf2
copy build\tests\_runner\dynamic\dynamic_tests_runner.uf2 executables\dynamic_tests_runner.uf2
copy build\tests\_runner\static\static_tests_runner.uf2 executables\static_tests_runner.uf2

popd