project(TopLevel)

add_library(toplevel SHARED WindowHandler.cpp)
target_include_directories(toplevel PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# add utility
target_link_libraries(toplevel PUBLIC utility)

# add states
target_link_libraries(toplevel PUBLIC states)

# add events
target_link_libraries(toplevel PUBLIC eventhandlers)

# linking with sfml
target_link_libraries(toplevel PUBLIC sfml-graphics)
target_compile_features(toplevel PUBLIC cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET toplevel POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:toplevel> $<TARGET_FILE_DIR:toplevel> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS toplevel)

set_target_properties(toplevel PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
