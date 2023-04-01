#!/bin/bash

cd $(dirname $0)/..

if [ ! -d build ]; then
   mkdir build
fi

cd build

cmake -G "Ninja" ..
ninja

cd ..

if [ ! -d executables ]; then
   mkdir executables
fi

cp build/core/ground_station/ground_station.uf2 executables/ground_station.uf2
cp build/core/satellite/satellite.uf2 executables/satellite.uf2
cp build/core/prototyping/prototyping.uf2 executables/prototyping.uf2
cp build/tests/crypto_tests/crypto_tests.uf2 executables/crypto_tests.uf2
cp build/tests/kernel_tests/kernel_tests.uf2 executables/kernel_tests.uf2