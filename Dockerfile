FROM ubuntu:latest


WORKDIR /rocket


RUN apt-get update && \
    apt-get install -y build-essential wget curl git python3

RUN apt-get -y install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi doxygen