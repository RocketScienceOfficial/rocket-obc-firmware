# Project Rocket

## Overview

Project for the rocket on board computer.

## Required Software

1. CMake
2. Ninja Build
3. ARM Toolchain
4. A compiler (GCC, Clang, MinGW, etc.)
5. Python 3
6. Docker

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

## Naming Conventions

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
- Global Variables -> <ins>Don't use them!</ins>
- Pointers -> p_camelCase
- Constants -> SCREAMING_SNAKE_CASE
- Macros -> SCREAMING_SNAKE_CASE