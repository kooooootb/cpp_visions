project(sfEvents)

add_library(sfevents SHARED sfEventComparator.cpp sfEventSerializer.cpp)
target_include_directories(sfevents PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(sfevents PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# linking with sfml
target_link_libraries(sfevents PUBLIC sfml-graphics) # sfml-graphics sfml-audio sfml-network sfml-sfevents sfml-system
target_compile_features(sfevents PUBLIC cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET sfevents POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:sfevents> $<TARGET_FILE_DIR:sfevents> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS sfevents)
