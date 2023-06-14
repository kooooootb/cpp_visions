project(Game)

add_library(game SHARED
        GameManager.cpp # main managers
        )
target_include_directories(game PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add factories
add_subdirectory(Factories)
target_link_libraries(game PUBLIC factories)

# add updaters
add_subdirectory(Updaters)
target_link_libraries(game PUBLIC updaters)

# add utility
target_link_libraries(game PUBLIC utility)

# add entities
target_link_libraries(game PUBLIC entities)

# add polygons
target_link_libraries(game PUBLIC polygons)

# linking with sfml
target_link_libraries(game PUBLIC sfml-graphics)
target_compile_features(game PUBLIC cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET game POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:game> $<TARGET_FILE_DIR:game> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS game)
