cmake_minimum_required(VERSION 3.13)
project(DesafioCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(CentralSystem src/CentralSystem.cpp src/Server.cpp src/LogWriter.cpp)