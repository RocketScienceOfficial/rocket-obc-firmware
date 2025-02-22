# Rocket On-Board Computer

## Overview
Rocket on-board computer software. Contains a lot of funcionality and can be used in very diverse situations.

## Features
 - Hardware Abstraction Layer (HAL) for different platforms (currently only RP2040)
 - Modular architecture
 - Variety of drivers for different sensors (IMUs, GPS, Barometers, etc.)
 - Flight State Machine to deploy parachutes
 - Diodes controllers
 - External voltage controllers
 - LoRa communication with Ground Station using DataLink protocol
 - Data saving to internal flash memory
 - Multidimensional Extended Kalman Filter for position and velocity estimation
 - Mahony and Madgwick filter for orientation estimation

## Getting Started

### Download
Run the following command to clone repo and initialize all submodules: ```git clone --recursive https://github.com/RocketScienceOfficial/rocket-obc-firmware```

If you already downloaded repo without ```--recursive``` flag, run: ```git submodule update --init --recursive```

### Environment
You have two ways to setup the project:

<ins>**1. Docker**</ins>

Build image: ```docker build -t rocket-firmware .```

Run container: ```docker run --rm -it -v %cd%:/rocket rocket-firmware```

<ins>**2. Manual**</ins>

Install required software:
1. CMake
2. ARM Toolchain
3. A build system (Ninja, etc...)
4. A compiler (GCC, Clang, MinGW, etc...)

### Build
Project is done in CMake, so create build directory and run: ```cmake ..``` in it. You can also specify your build system (e.g. Ninja) or set some variables (list of them is below). Then you can build your project: ```make``` (Linux) or for example ```ninja```.

Here is a list of supported CMake variables:
 - OBC_PLATFORM: only 'rp2040'
 - OBC_BOARD: 'obcv16'
 - CMAKE_BUILD_TYPE: Debug / Release