#!/bin/bash

cd `dirname $0`/..

sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib build-essential

git submodule update --init --recursive

scripts/build.sh