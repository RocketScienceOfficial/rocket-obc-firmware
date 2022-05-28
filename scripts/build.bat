@echo off

pushd %~dp0\..

if not exist "build" mkdir "build"

cd build

cmake -G "MinGW Makefiles" ..
make -j4
popd