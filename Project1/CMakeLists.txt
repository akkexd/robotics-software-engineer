cmake_minimum_required(VERSION 3.10)
project(build_my_world_plugin)

find_package(gazebo REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})

add_library(welcome_message SHARED script/welcome_message.cpp)
target_link_libraries(welcome_message ${GAZEBO_LIBRARIES})

