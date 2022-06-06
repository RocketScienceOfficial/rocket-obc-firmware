@echo off

pushd %~dp0\..

git submodule update --init

cd scripts
call build.bat
cd ..

cd server
call npm install
cd ..

popd