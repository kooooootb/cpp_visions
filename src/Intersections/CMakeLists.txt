project(Intersections)

add_library(intersections SHARED lineVsLine.cpp ManyLinesVsLine.cpp CirclesAndArcs.cpp)
target_include_directories(intersections PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(intersections PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add utility
target_link_libraries(intersections PUBLIC utility)
