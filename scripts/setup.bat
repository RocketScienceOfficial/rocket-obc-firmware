@echo off

pushd %~dp0\..

pushd server
npm install
popd

git submodule update --init --recursive
call build.bat

popd