project(Executors)
# this will do by event

add_library(executors SHARED Executor.cpp ExecutorsProvider.cpp)
target_include_directories(executors PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(executors PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# linking to commands
target_link_libraries(executors PUBLIC commands)

# link with utility
target_link_libraries(executors PUBLIC utility)

# link with sf events utilities
target_link_libraries(executors PUBLIC sfevents)

# linking with sfml
target_link_libraries(executors PUBLIC sfml-graphics) # sfml-graphics sfml-audio sfml-network sfml-executors sfml-system
target_compile_features(executors PUBLIC cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET executors POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:executors> $<TARGET_FILE_DIR:executors> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS executors)
