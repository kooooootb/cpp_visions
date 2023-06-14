project(States)

add_library(states SHARED GameState.cpp)
target_include_directories(states PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(states PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add polygons
target_link_libraries(states PUBLIC polygons)

# add utility
target_link_libraries(states PUBLIC utility)

# add entities
target_link_libraries(states PUBLIC entities)

# add game libs
target_link_libraries(states PUBLIC game)

# add event handlers
target_link_libraries(states PUBLIC eventhandlers)
