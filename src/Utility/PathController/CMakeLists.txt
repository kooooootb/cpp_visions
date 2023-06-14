project(PathController)

add_library(pathcontroller SHARED PathController.cpp)
target_include_directories(pathcontroller PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# set output directory
set_target_properties(pathcontroller PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
