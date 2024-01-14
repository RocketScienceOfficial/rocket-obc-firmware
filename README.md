# Project Rocket

## Overview
Rocket on-board computer software. Contains a lot of funcionality and can be used in very diverse situations.

## Features
 - Multidimensional Extended Kalman Filter
 - Flight State Machine
 - Variety of drivers for different sensors
 - Igniters controller
 - Commander system (mainly for telemetry use)

## Getting Started

### Required Software
1. CMake
2. ARM Toolchain
3. A build system (Ninja, etc...)
4. A compiler (GCC, Clang, MinGW, etc...)

### CMake Variables:
 - OBC_PLATFORM: only 'pico'
 - CMAKE_BUILD_TYPE: Debug / Release

## The Project
The rocket on-board computer project is a comprehensive initiative aimed at developing a sophisticated system to manage and control various aspects of a rocket's operation. 

The project encompasses five main components:
 1. On-Board Computer: https://github.com/Filipeak/rocket-obc
 2. Telemetry: https://github.com/Filipeak/rocket-tlm
 3. Estimation & Control Library: https://github.com/Filipeak/rocket-tlm
 4. Ground Station: https://github.com/Filipeak/rocket-gcs
 5. Hardware designs: https://github.com/Filipeak/rocket-hw