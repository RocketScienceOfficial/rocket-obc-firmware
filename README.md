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

## Programming Style

- Use provided types in api folder.
- Backend functions (drivers, etc.) should return FUNCRESULT.
- Always initialize local variables.

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
- Static Variables -> s_PascalCase
- Global Variables -> <ins>Don't use them!</ins>
- Pointers -> p_PascalCase
- Constants -> SCREAMING_SNAKE_CASE
- Macros -> SCREAMING_SNAKE_CASE

## Using Comments

- Use comments ONLY in header files or when it is REALLY necessary (but it should't be).
- Use @brief, @param, @return.
- Use /** */ for struct members.
- Don't add dots at the end of comments (unless there are more sentences).