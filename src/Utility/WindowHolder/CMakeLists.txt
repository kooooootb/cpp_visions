project(WindowHolder)

add_library(windowholder SHARED WindowHolder.cpp)
target_include_directories(windowholder PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# set output directory
set_target_properties(windowholder PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add points
target_link_libraries(windowholder PUBLIC trigonometry)

# add path control
target_link_libraries(windowholder PUBLIC pathcontroller)

# add data nodes
target_link_libraries(windowholder PUBLIC datanodes)

# linking with sfml
target_link_libraries(windowholder PUBLIC sfml-graphics) # sfml-graphics sfml-audio sfml-network sfml-windowholder sfml-system
target_compile_features(windowholder PUBLIC cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET windowholder POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:windowholder> $<TARGET_FILE_DIR:windowholder> COMMAND_EXPAND_LISTS)
endif()
