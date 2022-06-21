#!/bin/bash

cd `dirname $0`/../satellite

sudo cp executables/dataReceiver.uf2 /dev/sdb1
