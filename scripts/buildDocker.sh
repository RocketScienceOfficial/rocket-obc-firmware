#!/bin/bash

cd $(dirname $0)/../docker

docker build -t obc-env .
