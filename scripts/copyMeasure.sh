#!/bin/bash

cd `dirname $0`/..

sudo cp satellite/build/core/measure/measure.uf2 /dev/sdb1
