project(EventHandlers)

add_library(eventhandlers SHARED EventHandler.cpp GameEventHandler.cpp)
target_include_directories(eventhandlers PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(eventhandlers PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add commands
add_subdirectory(Commands)
target_link_libraries(eventhandlers PUBLIC commands)

# add main player's commands
target_link_libraries(eventhandlers PUBLIC mainplayercommands)

# add window's commands
target_link_libraries(eventhandlers PUBLIC windowcommands)

# add executors
add_subdirectory(Executors)
target_link_libraries(eventhandlers PUBLIC executors)

# add checkers
add_subdirectory(Checkers)
target_link_libraries(eventhandlers PUBLIC checkers)

# add utility
target_link_libraries(eventhandlers PUBLIC utility)

# add entities
target_link_libraries(eventhandlers PUBLIC entities)

# add polygons
target_link_libraries(eventhandlers PUBLIC polygons)

# add game libs
target_link_libraries(eventhandlers PUBLIC game)

# linking with sfml
target_link_libraries(eventhandlers PUBLIC sfml-graphics)
target_compile_features(eventhandlers PUBLIC cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET eventhandlers POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:eventhandlers> $<TARGET_FILE_DIR:eventhandlers> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS eventhandlers)
