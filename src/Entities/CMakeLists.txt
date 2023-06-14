project(Entities)

add_library(entities SHARED
        Weapon.cpp Projectile.cpp
        Enemy.cpp MainPlayer.cpp
        Entity.cpp SpriteHolder.cpp
        Alive.cpp Movable.cpp Rotatable.cpp Armed.cpp Visioning.cpp Pickable.cpp
        EntityStorage.cpp
        )
target_include_directories(entities PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# set output directory
set_target_properties(entities PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# add utility
target_link_libraries(entities PUBLIC utility)

# polygons
target_link_libraries(entities PUBLIC polygons)

# and intersections
target_link_libraries(entities PUBLIC intersections)

# linking with sfml
target_link_libraries(entities PUBLIC sfml-graphics) # sfml-graphics sfml-audio sfml-network sfml-entities sfml-system
target_compile_features(entities PUBLIC cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET entities POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:entities> $<TARGET_FILE_DIR:entities> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS entities)
