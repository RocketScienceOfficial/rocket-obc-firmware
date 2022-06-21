#!/bin/bash

cd `dirname $0`/../satellite

if [ ! -d build ]; then
   mkdir build
fi

cd build

cmake -G "Unix Makefiles" ..
make

cd ..

if [ ! -d executables ]; then
   mkdir executables
fi

cp build/core/dataReceiver/dataReceiver.uf2 executables/dataReceiver.uf2
cp build/core/measure/measure.uf2 executables/measure.uf2
cp build/tests/_runner/dynamic/dynamic_tests_runner.uf2 executables/dynamic_tests_runner.uf2
cp build/tests/_runner/static/static_tests_runner.uf2 executables/static_tests_runner.uf2