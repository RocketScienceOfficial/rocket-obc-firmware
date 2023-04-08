@echo off

pushd %~dp0\..

git submodule update --init --recursive

call build.bat
call pip install -r requirements.txt

popd
