#!/bin/bash

cd `dirname $0`/../satellite

if [ ! -d build ]; then
   mkdir build
fi

cd build

cmake -G "Unix Makefiles" ..
make