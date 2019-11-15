cmake_minimum_required(VERSION 2.8.3)
project(grid_sim)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-std=c++11)

find_package(Threads)

find_package(catkin REQUIRED capnzero alica_capnzero_proxy id_capnzero id_manager fsystem system_config cnc_geometry srg_world)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}_msgs
  CATKIN_DEPENDS capnzero alica_capnzero_proxy id_capnzero id_manager fsystem system_config cnc_geometry srg_world
  #DEPENDS SFML
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

############### Simulator
add_executable(${PROJECT_NAME}
  src/srg/Simulator.cpp
  src/srg/sim/communication/Communication.cpp
  src/srg/sim/commands/SpawnCommandHandler.cpp
  src/srg/sim/commands/CommandHandler.cpp
  src/srg/sim/commands/MoveCommandHandler.cpp
  src/srg/sim/commands/ManipulationHandler.cpp
  src/srg/sim/Arm.cpp
  src/srg/sim/Sensor.cpp
  src/srg/sim/SimulatedRobot.cpp
)

target_link_libraries(${PROJECT_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
  ${PROJECT_NAME}_msgs
  ${catkin_LIBRARIES}
)

############### Sim Messages
find_package(CapnProto REQUIRED)
set(CAPNPC_IMPORT_DIRS ${id_capnzero_SOURCE_PREFIX}/include)# include external msgs
set(${PROJECT_NAME}_msgdir "${CMAKE_CURRENT_SOURCE_DIR}/include/srg/sim/msgs")
file(GLOB_RECURSE capnp_messages ${${PROJECT_NAME}_dir} *.capnp)
set(CAPNPC_SRC_PREFIX ${${PROJECT_NAME}_msgdir})
set(CAPNPC_OUTPUT_DIR ${${PROJECT_NAME}_msgdir})
capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${capnp_messages})

add_library(${PROJECT_NAME}_msgs
        src/srg/sim/ContainerUtils.cpp
  ${CAPNP_SRCS}
)
target_link_libraries(${PROJECT_NAME}_msgs
  ${catkin_LIBRARIES}
)
