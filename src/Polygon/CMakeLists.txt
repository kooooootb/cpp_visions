project(Polygon)

add_library(polygons SHARED Polygon.cpp Bbox.cpp VertexHolder.cpp)
target_include_directories(polygons PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(polygons PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add utility
target_link_libraries(polygons PUBLIC utility)
