#!/bin/bash

cd `dirname $0`/..

sudo cp build/core/dataReceiver/dataReceiver.uf2 /dev/sdb1
