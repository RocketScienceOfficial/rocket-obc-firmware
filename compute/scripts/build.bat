@echo off
pushd %~dp0\..\build
cmake -G "MinGW Makefiles" ..
make
popd