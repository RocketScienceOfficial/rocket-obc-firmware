@echo off

pushd %~dp0\..

call docker run --rm -it -v %cd%:/root/env obc-env

popd