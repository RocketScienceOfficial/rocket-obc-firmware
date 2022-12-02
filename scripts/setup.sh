#!/bin/bash

cd $(dirname $0)/..

sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib build-essential ninja-build

git submodule update --init --recursive

./obc/scripts/build.sh

pip install -r ./obc/utils/requirements.txt
