project(MainMenu)

add_library(mainmenu SHARED MainMenu.cpp)
target_include_directories(mainmenu PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
