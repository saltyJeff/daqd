cmake_minimum_required(VERSION 3.11)
project(daq_daemon)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(./include)
add_subdirectory(./sockpp)
add_subdirectory(spdlog)
find_library(modbus libmodbus CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)

add_executable(daq_daemon ${SRC_FILES})
target_link_libraries(daq_daemon sockpp_lib spdlog Threads::Threads modbus)

set_target_properties(daq_daemon PROPERTIES OUTPUT_NAME "daqd")

install (TARGETS daq_daemon RUNTIME DESTINATION bin)