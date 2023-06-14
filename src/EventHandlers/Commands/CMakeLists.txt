project(Commands)

add_library(commands SHARED Command.cpp)
target_include_directories(commands PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(commands PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_subdirectory(MainPlayerCommands)
add_subdirectory(WindowCommands)
