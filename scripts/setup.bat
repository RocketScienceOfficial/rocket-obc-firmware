@echo off

pushd %~dp0\..

git submodule update --init --recursive

cd obc\scripts

call build.bat

cd ..\utils

pip install -r requirements.txt

cd ..\..

popd
