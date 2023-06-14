project(Updaters)

add_library(updaters SHARED
        Updater.cpp
        )
target_include_directories(updaters PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(updaters PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add utility
target_link_libraries(updaters PUBLIC utility)
