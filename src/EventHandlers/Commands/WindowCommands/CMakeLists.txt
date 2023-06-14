project(WindowCommands)

add_library(windowcommands SHARED WindowCommand.cpp
        Close.cpp Resize.cpp
        )
target_include_directories(windowcommands PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(windowcommands PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add base command
target_link_libraries(windowcommands PUBLIC commands)

# add window and other utility libs
target_link_libraries(windowcommands PUBLIC utility)
