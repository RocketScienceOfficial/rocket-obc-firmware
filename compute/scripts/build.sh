#!/bin/bash
pushd ../build
cmake -G "MinGW Makefiles" ..
make
popd