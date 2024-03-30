# Project Rocket

## Overview
Rocket on-board computer software. Contains a lot of funcionality and can be used in very diverse situations.

## Features
 - Multidimensional Extended Kalman Filter
 - Flight State Machine
 - Variety of drivers for different sensors
 - Igniters controller

## Getting Started

### Download
Run the following command to clone repo and initialize all submodules: ```git clone --recursive https://github.com/Filipeak/rocket-obc-firmware```

If you already downloaded repo without ```--recursive``` flag, run: ```git submodule update --init```

### Environment
You have two ways to setup the project:

<ins>**1. Docker**</ins>

Build image: ```docker build -t rocket-firmware .```

Run container: ```docker run --rm -it -v %cd%:/rocket rocket-firmware```

<ins>**2. Manual**</ins>

Install requireed software:
1. CMake
2. ARM Toolchain
3. A build system (Ninja, etc...)
4. A compiler (GCC, Clang, MinGW, etc...)

### Build
Project is done in CMake, so create build directory and run: ```cmake ..``` in it. You can also specify your build system (e.g. Ninja) or set some variables. Then you can build your project: ```make``` (Linux) or for example ```ninja```.

Here is a list of supported CMake variables:
 - OBC_PLATFORM: only 'pico'
 - CMAKE_BUILD_TYPE: Debug / Release

## The Project
The rocket on-board computer project is a comprehensive initiative aimed at developing a sophisticated system to manage and control various aspects of a rocket's operation. 

The project encompasses five main components:
 1. [On-Board Computer](https://github.com/Filipeak/rocket-obc-firmware)
 2. [On-Board Hardware Designs](https://github.com/Filipeak/rocket-obc-hardware)
 3. [Telemetry](https://github.com/Filipeak/rocket-telemetry)
 4. [Estimation & Control Library](https://github.com/Filipeak/rocket-ecl)
 5. [Ground Station](https://github.com/Filipeak/rocket-gcs)