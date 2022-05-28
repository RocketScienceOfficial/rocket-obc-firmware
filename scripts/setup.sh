#!/bin/bash

cd `dirname $0`/..

sudo apt install node cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib build-essential

cd server
npm install
cd -

git submodule update --init --recursive

scripts/build.sh