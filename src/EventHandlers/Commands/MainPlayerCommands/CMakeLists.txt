project(MainPlayerCommands)

add_library(mainplayercommands SHARED MainPlayerCommand.cpp
        MoveUp.cpp MoveDown.cpp MoveLeft.cpp MoveRight.cpp # Checkers
        Rotate.cpp Pickup.cpp # Executors
        )
target_include_directories(mainplayercommands PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(mainplayercommands PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add base command
target_link_libraries(mainplayercommands PUBLIC commands)

# add main player via entities link
target_link_libraries(mainplayercommands PUBLIC entities)
