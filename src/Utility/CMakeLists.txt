project(Utility)

add_library(utility SHARED Watcher.cpp)
target_include_directories(utility PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# add utility namespace
add_library(utilityns SHARED Utility.cpp)
target_include_directories(utilityns PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(utility PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add window wrapper
add_subdirectory(WindowHolder)
target_link_libraries(utility PUBLIC windowholder)

# add path controller
add_subdirectory(PathController)
target_link_libraries(utility PUBLIC pathcontroller)

# add common interfaces
add_subdirectory(CommonInterfaces)
target_link_libraries(utility PUBLIC commoninterfaces)

# add data nodes
add_subdirectory(DataNodes)
target_link_libraries(utility PUBLIC datanodes)

# add trigonometry units
add_subdirectory(Trigonometry)
target_link_libraries(utility PUBLIC trigonometry)

# linking with sfml
target_link_libraries(utility PUBLIC sfml-graphics) # sfml-graphics sfml-audio sfml-network sfml-utility sfml-system
target_compile_features(utility PUBLIC cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET utility POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:utility> $<TARGET_FILE_DIR:utility> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS utility)
