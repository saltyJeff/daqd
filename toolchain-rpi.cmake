# Define our host system
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)
# Define the cross compiler locations
SET(CMAKE_C_COMPILER   /opt/rpi/x64-gcc-8.3.0/arm-rpi-linux-gnueabihf/bin/arm-rpi-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER /opt/rpi/x64-gcc-8.3.0/arm-rpi-linux-gnueabihf/bin/arm-rpi-linux-gnueabihf-g++)
# Define the sysroot path for the RaspberryPi distribution in our tools folder 
SET(CMAKE_FIND_ROOT_PATH /opt/rpi/x64-gcc-8.3.0/arm-rpi-linux-gnueabihf/arm-rpi-linux-gnueabihf/sysroot)
# Use our definitions for compiler tools
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search for libraries and headers in the target directories only
# SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set (CMAKE_CXX_FLAGS "-fsigned-char")