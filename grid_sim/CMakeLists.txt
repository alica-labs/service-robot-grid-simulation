cmake_minimum_required(VERSION 2.8.3)
project(grid_sim)

add_compile_options(-std=c++11)

# SFML (until 2.5 you need an extra FindSFML.cmake aside your project...)
# https://github.com/SFML/SFML/wiki/Tutorial:-Build-your-SFML-project-with-CMake
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 COMPONENTS network audio graphics window system REQUIRED)

find_package(Threads)

find_package(catkin REQUIRED)
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  DEPENDS SFML
)

include_directories(
  include
  ${SFML_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME}
  src/Simulator.cpp
  src/World.cpp
  src/Cell.cpp
  src/GUI.cpp
)

target_link_libraries(${PROJECT_NAME}
  ${SFML_LIBRARIES}
  ${SFML_DEPENDENCIES}
  ${CMAKE_THREAD_LIBS_INIT}
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

