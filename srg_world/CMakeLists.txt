cmake_minimum_required(VERSION 2.8.3)
project(srg_world)

add_compile_options(-std=c++11)
set(CMAKE_BUILD_TYPE Debug)

#SFML(until 2.5 you need an extra FindSFML.cmake aside your project...)
#https: // github.com/SFML/SFML/wiki/Tutorial:-Build-your-SFML-project-with-CMake
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 COMPONENTS graphics window system REQUIRED)

find_package(tmxparser REQUIRED)

find_package(catkin REQUIRED id_manager system_config)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES srg_world srg_viz
  CATKIN_DEPENDS id_manager system_config
  DEPENDS SFML
)

include_directories(
  include
  ${SFML_INCLUDE_DIR}
  ${catkin_INCLUDE_DIRS}
)

###### WORLD REPRESENTATION
add_library(${PROJECT_NAME}
  src/srg/World.cpp
  src/srg/world/Cell.cpp
  src/srg/world/Coordinate.cpp
  src/srg/world/Object.cpp
  src/srg/world/Door.cpp
        src/srg/world/Agent.cpp
  src/srg/world/ObjectState.cpp
  src/srg/world/Room.cpp
  src/srg/world/ObjectType.cpp
  src/srg/world/RoomType.cpp
  src/srg/world/Direction.cpp
        src/srg/world/ObjectSet.cpp include/srg/world/ObjectSet.h)
target_link_libraries(${PROJECT_NAME}
  tmxparser
  ${catkin_LIBRARIES}
)

###### VISUALISATION
add_library(srg_viz
  src/srg/GUI.cpp
  src/srg/viz/Marker.cpp
  src/srg/viz/SpriteType.cpp
)
target_link_libraries(srg_viz
  ${SFML_LIBRARIES}
  ${SFML_DEPENDENCIES}
  ${catkin_LIBRARIES}
)

