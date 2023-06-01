# Project Rocket

## Overview

Project for the rocket system.

## Required Software

***

1. CMake
2. Ninja
3. ARM Toolchain
4. A compiler (GCC, Clang, MinGW, etc.)
5. Docker

## Install

***

1. Download the source code from GitHub or run the following command:

``` console
$ git clone https://github.com/Filipeak/project-rocket.git
```

2. Run setup script:

``` console
$ ./setup.sh
```

3. Naming
- Struct -> PascalCase
- Struct Members -> camelCase
- Union -> PascalCase
- Union Members -> camelCase
- Enum -> PascalCase
- Enum Members -> SCREAMING_SNAKE_CASE
- Typedef -> PascalCase
- Functions -> camelCase
- Local Variables -> camelCase
- Static Variables -> s_camelCase
- Global Variables -> Don't use them!
- Pointers -> p_camelCase
- Constants -> SCREAMING_SNAKE_CASE
- Macros -> SCREAMING_SNAKE_CASE