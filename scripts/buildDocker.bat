@echo off

pushd %~dp0\..

call docker build -t obc-env .

popd