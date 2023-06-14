project(Factories)

add_library(factories SHARED
        WeaponFactory.cpp EnemyFactory.cpp)
target_include_directories(factories PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(factories PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add utility
target_link_libraries(factories PUBLIC utility)

# add entities
target_link_libraries(factories PUBLIC entities)

# add polygons
target_link_libraries(factories PUBLIC polygons)
