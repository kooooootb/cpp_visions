cmake_minimum_required(VERSION 3.20)
project(t5)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ../files)

add_executable(t5 Common.cpp main.cpp Polygon.cpp Polygon.cpp Player.cpp Game.cpp Entity.cpp Weapon.cpp Projectile.cpp)

add_executable(levelEditor level.cpp Common.cpp Polygon.cpp Polygon.h)

add_executable(entityEditor EntityEditor.cpp)

#sfml libs
set(BUILD_SHARED_LIBS FALSE)

add_subdirectory(sfml)
include_directories(sfml/include)

target_link_libraries(t5 sfml-graphics sfml-audio sfml-network sfml-window sfml-system)
target_link_libraries(levelEditor sfml-graphics sfml-audio sfml-network sfml-window sfml-system)
target_link_libraries(entityEditor sfml-graphics sfml-audio sfml-network sfml-window sfml-system)

##nanoflann libs
#add_subdirectory(nanoflann-master)
#include_directories(nanoflann-master/include)
#target_link_libraries(t5 nanoflann::nanoflann)
