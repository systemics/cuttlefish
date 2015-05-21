set(RASPBERRY 1)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_COMPILER   ${CMAKE_SOURCE_DIR}/cross/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/cross/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-g++)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SOURCE_DIR}/cross/pi-root)
#set(CMAKE_SYSROOT ${CMAKE_SOURCE_DIR}/cross/pi-root)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(DEFINITIONS "-D __raspberry__")
