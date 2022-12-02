#!/bin/bash

cd $(dirname $0)/..

docker run --rm -it -v %pwd%:/root/env obc-env
