set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_COMPILER $ENV{HOME}/code/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER $ENV{HOME}/code/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-c++)
set(CMAKE_FIND_ROOT_PATH $ENV{HOME}/code/pi-root)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_CXX_FLAGS "-static-libstdc++")