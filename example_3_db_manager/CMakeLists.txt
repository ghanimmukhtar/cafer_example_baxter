cmake_minimum_required(VERSION 2.8.3)
project(example_3_db_manager)

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
set(EXPERIMENT_SOURCE_FILES src/db_manager_node.cpp)

catkin_package(
 INCLUDE_DIRS include
#  LIBRARIES example_wave_1
#  CATKIN_DEPENDS cafer_core roscpp rospy std_msgs tf
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(db_manager_node ${EXPERIMENT_SOURCE_FILES} )
target_link_libraries(db_manager_node ${catkin_LIBRARIES} yaml-cpp.so)
