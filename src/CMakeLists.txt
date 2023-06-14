project(src)

# add directories
add_subdirectory(Entities)
add_subdirectory(Game)
# add_subdirectory(LevelEditor)
# add_subdirectory(MainMenu)
add_subdirectory(Polygon)
add_subdirectory(Utility)
add_subdirectory(Intersections)
add_subdirectory(TopLevel)
add_subdirectory(States)
add_subdirectory(EventHandlers)
add_subdirectory(sfEvents)

# load things
include(FetchContent)

# load sfml
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# load jsonparser
FetchContent_Declare(JSONCppParser
        GIT_REPOSITORY https://github.com/kooooootb/JSONCppParser.git)
FetchContent_MakeAvailable(JSONCppParser)

# configure main executable
add_executable(visions main.cpp)
target_link_libraries(visions PRIVATE toplevel)

# set output directory
set_target_properties(visions PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
