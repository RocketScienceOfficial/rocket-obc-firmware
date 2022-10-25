#!/bin/bash

cd `dirname $0`/../satellite

sudo cp executables/ground_station.uf2 /dev/sdb1
