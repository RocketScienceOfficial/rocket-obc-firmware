@echo off

pushd %~dp0\..

git submodule update --init --recursive

cd scripts
call build.bat

pip install -r requirements.txt
cd ..

popd
