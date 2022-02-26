@echo off
pushd %~dp0
call build
call upload
popd