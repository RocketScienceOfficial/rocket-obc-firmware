@echo off

pushd %~dp0\..\docker

call docker build -t obc-env .

popd