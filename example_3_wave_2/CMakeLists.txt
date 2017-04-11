cmake_minimum_required(VERSION 2.8.3)
project(example_3_wave_2)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  cafer_core
  roscpp
  rospy
  std_msgs
  tf
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fPIC")

catkin_package(
 INCLUDE_DIRS include
#  LIBRARIES example_wave_1
#  CATKIN_DEPENDS cafer_core roscpp rospy std_msgs tf
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(wave_2_node src/wave_2_node.cpp)
target_link_libraries(wave_2_node ${catkin_LIBRARIES} yaml-cpp.so)
