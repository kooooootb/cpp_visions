project(Trigonometry)

add_library(trigonometry SHARED Point.cpp Vector.cpp)
target_include_directories(trigonometry PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# set output directory
set_target_properties(trigonometry PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

target_link_libraries(trigonometry PUBLIC utilityns)

# linking with sfml
target_link_libraries(trigonometry PUBLIC sfml-graphics) # sfml-graphics sfml-audio sfml-network sfml-utility sfml-system
target_compile_features(trigonometry PUBLIC cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET trigonometry POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:trigonometry> $<TARGET_FILE_DIR:trigonometry> COMMAND_EXPAND_LISTS)
endif()
