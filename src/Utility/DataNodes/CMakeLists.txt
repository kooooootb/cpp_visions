project(DataNodes)

add_library(datanodes SHARED DataNode.cpp JSONDataNode.cpp)
target_include_directories(datanodes PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# set output directory
set_target_properties(datanodes PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# linking with jsonparser
target_link_libraries(datanodes PUBLIC jsonparser)

# linking with trigonometry
target_link_libraries(datanodes PUBLIC trigonometry)
