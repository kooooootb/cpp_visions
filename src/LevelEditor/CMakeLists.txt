project(LevelEditor)

set(Target_name leveleditor)

add_library(${Target_name} SHARED LevelEditor.cpp)
target_include_directories(${Target_name} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# add polygons
target_link_libraries(${Target_name} PUBLIC polygon)

# add utilities
target_link_libraries(${Target_name} PUBLIC utility)

# linking with sfml
target_link_libraries(${Target_name} PUBLIC sfml-graphics) # sfml-graphics sfml-audio sfml-network sfml-${Target_name} sfml-system
target_compile_features(${Target_name} PUBLIC cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET ${Target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${Target_name}> $<TARGET_FILE_DIR:${Target_name}> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS ${Target_name})

