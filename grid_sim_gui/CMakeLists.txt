cmake_minimum_required(VERSION 2.8.3)
project(grid_sim_gui)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED)
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES grid_sim_gui
#  DEPENDS system_lib
)

include_directories(
  include
)

add_library(${PROJECT_NAME}
  src/GridSimGUI.cpp
)

