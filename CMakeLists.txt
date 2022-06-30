cmake_minimum_required(VERSION 3.20)
project(t5)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ../files)

add_executable(t5 main.cpp file.cpp Common.cpp Polygon.cpp Polygon.h Polygon.cpp)

add_executable(level level.cpp file.cpp Common.cpp Polygon.cpp Polygon.h)

#sfml libs
set(BUILD_SHARED_LIBS FALSE)

add_subdirectory(sfml)
include_directories(sfml/include)

target_link_libraries(t5 sfml-graphics sfml-audio sfml-network sfml-window sfml-system)
target_link_libraries(level sfml-graphics sfml-audio sfml-network sfml-window sfml-system)

add_subdirectory(nanoflann-master)
include_directories(nanoflann-master/include)
target_link_libraries(t5 nanoflann::nanoflann)
