#!/bin/bash

cd `dirname $0`/..

sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential

git submodule update --init --recursive
