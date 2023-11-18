# Project Rocket

## Overview

Project for the rocket on board computer.

## Required Software

1. CMake
2. ARM Toolchain
3. A build system (Ninja, etc...)
4. A compiler (GCC, Clang, MinGW, etc...)

## Install

1. Download the source code from GitHub or run the following command:

``` console
$ git clone https://github.com/Filipeak/project-rocket.git
```

2. Run setup script:

   - Windows:
   ``` console
   $ scripts/setup.bat
   ```
    - Linux:
   ``` console
   $ ./scripts/setup.sh
   ```
3. You can also use docker to build the project:
    - Windows:
   ``` console
   $ scripts/buildDocker.bat
   ```
    - Linux:
   ``` console
   $ ./scripts/buildDocker.sh
   ```

   and then call a script to enter the container:
     - Windows:
   ``` console
   $ scripts/runEnv.bat
   ```
    - Linux:
   ``` console
   $ ./scripts/runEnv.sh
   ```