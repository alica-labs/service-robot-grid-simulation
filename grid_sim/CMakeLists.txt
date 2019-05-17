cmake_minimum_required(VERSION 2.8.3)
project(grid_sim)

add_compile_options(-std=c++11)

# SFML (until 2.5 you need an extra FindSFML.cmake aside your project...)
# https://github.com/SFML/SFML/wiki/Tutorial:-Build-your-SFML-project-with-CMake
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 COMPONENTS graphics window system REQUIRED)

find_package(Threads)

find_package(catkin REQUIRED capnzero)
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME} ${PROJECT_NAME}_msgs ${PROJECT_NAME}_world
  DEPENDS SFML
)

include_directories(
  include
  ${SFML_INCLUDE_DIR}
  ${catkin_INCLUDE_DIRS}
)

############### Simulator

add_executable(${PROJECT_NAME}
  src/Simulator.cpp
  src/GUI.cpp
)

target_link_libraries(${PROJECT_NAME}
  ${SFML_LIBRARIES}
  ${SFML_DEPENDENCIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${PROJECT_NAME}_msgs
  ${PROJECT_NAME}_world
  ${catkin_LIBRARIES}
)

# copy the resource (e.g. texture files)
file(GLOB ResourceFiles ${CMAKE_SOURCE_DIR}/textures/*.png)
add_custom_target(copy ALL
  DEPENDS
  ${ResourceFiles}
)
foreach(ResourceFile ${ResourceFiles})
  get_filename_component(file ${ResourceFile} NAME)
  add_custom_command(TARGET copy
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy ${ResourceFile} $<TARGET_FILE_DIR:grid_sim>/textures/${file}
    COMMAND ${CMAKE_COMMAND} -E
    make_directory $<TARGET_FILE_DIR:grid_sim>/textures/
  )
endforeach()

add_library(${PROJECT_NAME}_world
  src/World.cpp
  src/Cell.cpp
  src/Coordinate.cpp
)

############### Sim Messages

find_package(CapnProto REQUIRED)
set(grid_sim_message_folder "${CMAKE_CURRENT_SOURCE_DIR}/msg")
file(GLOB_RECURSE grid_sim_msgs ${grid_sim_message_folder} *.capnp)
set(CAPNPC_SRC_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/msg)
file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/grid_sim_msgs")
set(CAPNPC_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/grid_sim_msgs")
capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${grid_sim_msgs})

add_library(${PROJECT_NAME}_msgs
  ${CAPNP_SRCS}
)
