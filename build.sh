#!/bin/bash

cd `dirname $0`

if [ ! -d build ]; then
   mkdir build
fi

cd build

cmake -G "Unix Makefiles" ..
make
