cmake_minimum_required(VERSION 3.16)
project(robot_control_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(robot_core
    manager/robot.cpp
    manager/robot_manager.cpp
    manager/managed_robot.cpp
    manager/multi_robot_manager.cpp
    protocol/parser.cpp
)

target_include_directories(robot_core PUBLIC
    manager
    protocol
    host/cpp
)

add_subdirectory(host/cpp)
add_subdirectory(examples)
