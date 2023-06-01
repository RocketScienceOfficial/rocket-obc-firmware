FROM ubuntu:latest

RUN apt-get update && apt-get upgrade -y && apt install git gdb-multiarch cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib build-essential ninja-build python3 -y

VOLUME /root/env
WORKDIR /root/env