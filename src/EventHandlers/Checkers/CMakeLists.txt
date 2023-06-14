project(Checkers)
# this will check and perform every iteration

add_library(checkers SHARED Checker.cpp CheckersProvider.cpp)
target_include_directories(checkers PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(checkers PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# linking to commands
target_link_libraries(checkers PUBLIC commands)

# link with utility
target_link_libraries(checkers PUBLIC utility)

# link with sf events utilities
target_link_libraries(checkers PUBLIC sfevents)

# linking with sfml
target_link_libraries(checkers PUBLIC sfml-graphics) # sfml-graphics sfml-audio sfml-network sfml-checkers sfml-system
target_compile_features(checkers PUBLIC cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET checkers POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:checkers> $<TARGET_FILE_DIR:checkers> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS checkers)
