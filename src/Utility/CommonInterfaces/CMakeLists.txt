project(CommonInterfaces)

add_library(commoninterfaces SHARED Updatable.cpp)
target_include_directories(commoninterfaces PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# set output directory
set_target_properties(commoninterfaces PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add window wrapper
target_link_libraries(utility PUBLIC windowholder)
