cmake_minimum_required(VERSION 3.11)
project(daq_daemon)

set(CMAKE_CXX_STANDARD 17)
option(SOCKPP_BUILD_STATIC ON)
option(SOCKPP_BUILD_SHARED OFF)
include_directories(./include)
add_subdirectory(sockpp)
add_subdirectory(spdlog)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/**/*.cpp ${PROJECT_SOURCE_DIR}/src/**/*.h)
add_executable(daq_daemon ${SRC_FILES})

# add_executable(daq_daemon src/dataclasses/DaqConfigJson.h src/dataclasses/DeviceJson.h src/dataclasses/PollMode.h src/dataclasses/ItemJson.h src/DaqState.cpp src/DaqState.h src/COMPILE_OPTS.h src/main.cpp src/devices/SineDevice.cpp src/devices/SineDevice.h src/DaqItem.cpp src/utils.h src/utils.cpp src/poll.cpp src/poll.h src/socketHandler.h src/socketHandler.cpp)
target_link_libraries(daq_daemon sockpp spdlog)

install (TARGETS daq_daemon RUNTIME DESTINATION bin)