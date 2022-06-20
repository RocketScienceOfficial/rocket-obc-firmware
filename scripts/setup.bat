@echo off

pushd %~dp0\..

git submodule update --init --recursive

cd scripts
call build.bat
cd ..

cd presentation\server
call npm install
cd ..

popd
