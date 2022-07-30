@echo off

pushd %~dp0\..\telemetry\server

call npm run start-dev

popd