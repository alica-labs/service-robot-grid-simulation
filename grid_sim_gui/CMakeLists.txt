cmake_minimum_required(VERSION 2.8.3)
project(grid_sim_gui)

add_compile_options(-std=c++11)

# SFML (until 2.5 you need an extra FindSFML.cmake aside your project...)
# https://github.com/SFML/SFML/wiki/Tutorial:-Build-your-SFML-project-with-CMake
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 COMPONENTS network audio graphics window system REQUIRED)

find_package(catkin REQUIRED)
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES grid_sim_gui
  DEPENDS SFML
)

include_directories(
  include
  ${SFML_INCLUDE_DIR}
)

############# The library, the actual simulator can use

add_library(${PROJECT_NAME}
  src/GridSimGUI.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${SFML_LIBRARIES}
  ${SFML_DEPENDENCIES}
)

############## A stand alone version, for testing purposes

add_executable(${PROJECT_NAME}_standalone
  src/GridSimGUI.cpp
)

target_link_libraries(${PROJECT_NAME}_standalone
  ${SFML_LIBRARIES}
  ${SFML_DEPENDENCIES}
)

