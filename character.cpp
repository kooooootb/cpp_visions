#include <iostream>
#include <fstream>

#include "headers.h"

template<typename T>
void rewrite(std::string &fname) {
    std::ofstream fd(fname, std::ios::trunc | std::ios::binary);

    int i = 0;
    T arg[argLength];

    for (const auto &mes: argNames) {
        std::cout << mes << ":";
        std::cin >> arg[i++];
    }

    fd.seekp(SEEK_SET);
    fd.write(reinterpret_cast<char *>(arg), sizeof(arg[0]) * argLength);
}

template<typename T>
void edit(std::string &fname) {
    std::ifstream ifd(fname, std::ios::binary);

    int i;
    T arg[argLength];

    ifd.seekg(SEEK_SET);
    ifd.read(reinterpret_cast<char *>(arg), sizeof(arg[0]) * argLength);

    std::cout << "Choose argument to change:";
    i = 0;
    for (const auto &mes: argNames) {
        std::cout << std::to_string(i++) << " - " << mes << ' ';
    }

    std::cout << std::endl;
    std::cin >> i;
    std::cout << "Input new value" << std::endl;
    std::cin >> arg[i];

    ifd.close();
    std::ofstream ofd(fname, std::ios::binary);

    ofd.seekp(SEEK_SET);
    for (i = 0; i < argLength; ++i) {
        ofd.write(reinterpret_cast<char *>(arg), sizeof(arg[0]) * argLength);
    }

    ofd.close();
}

template<typename T>
void print(std::string &fname) {
    std::ifstream fd(fname, std::ios::binary);

    int i;
    T arg[argLength];

    fd.seekg(SEEK_SET);
    if (fd.read(reinterpret_cast<char *>(arg), sizeof(arg[0]) * argLength).eof()) {
        std::cout << "File empty" << std::endl;
        return;
    }

    i = 0;
    for (const auto &mes: argNames) {
        std::cout << mes << ": " << arg[i++] << std::endl;
    }

    fd.close();
}

int main(){
    std::string fname;
    int choice = 1;

    void (*fptr[])(std::string &) = {nullptr, rewrite<float>, edit<float>, print<float>};

    std::cout << "Input file's name:";
    std::cin >> fname;
    std::fstream fd(fname, std::ios::in);

    if(!fd.is_open()){
        std::cout << "Can't open file, creating new" << std::endl;
        fd.open(fname, std::ios::out | std::ios::trunc);
    }

    fd.close();

    while(choice != 0){
        for(const auto &mes : messages){
            std::cout << mes << std::endl;
        }
        std::cout << "Your choice:";
        std::cin >> choice;
        if(choice != 0) {
            fptr[choice](fname);
        }
    }

    return 0;
}