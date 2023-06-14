#include <filesystem>
#include <iostream>
#include <fstream>

#include "WindowHandler.h"

int main(int argc, char* argv[]){
    std::filesystem::path rootFolder(argv[0]);
    rootFolder = rootFolder.parent_path();

    WindowHandler windowHandler(rootFolder);
    windowHandler.run();

    return 0;
}