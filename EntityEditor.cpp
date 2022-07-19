#include <iostream>
#include <fstream>

#include "Common.h"

const std::vector<std::string> *argNames;
const unsigned long long *argLength;

void writeFname(std::string &fname, std::ofstream &fd){
    unsigned long long size = fname.size();
    fd.write(reinterpret_cast<char *>(&size), sizeof(size));
    fd.write(reinterpret_cast<char *>(&fname[0]), sizeof(fname[0]) * size);
}

void writeFname(std::ofstream &fd){
    std::string fname;
    std::cout << "Input entity's sprite file name:";
    std::cin >> fname;

    writeFname(fname, fd);
}

std::string readFname(std::ifstream &fd){
    std::string res;

    unsigned long long size;

    fd.read(reinterpret_cast<char *>(&size), sizeof(size));

    res.resize(size);

    fd.read(reinterpret_cast<char *>(&res[0]), sizeof(char) * size);

    return res;
}

template<typename T>
void rewrite(std::string &fname) {
    std::ofstream fd(fname, std::ios::trunc | std::ios::binary);

    int i = 0;
    T arg[*argLength];

    for (const auto &mes: *argNames) {
        std::cout << mes << ":";
        std::cin >> arg[i++];
    }

    fd.seekp(SEEK_SET);
    fd.write(reinterpret_cast<char *>(arg), sizeof(arg[0]) * *argLength);

    writeFname(fd);

    fd.close();
}

template<typename T>
void edit(std::string &fname) {
    std::ifstream ifd(fname, std::ios::binary);

    int i;
    T arg[*argLength];

    ifd.seekg(SEEK_SET);
    ifd.read(reinterpret_cast<char *>(arg), sizeof(arg[0]) * *argLength);
    std::string spriteName = readFname(ifd);

    std::cout << "Choose argument to change:";
    i = 0;
    for (const auto &mes: *argNames) {
        std::cout << std::to_string(i++) << " - " << mes << ' ';
    }
    std::cout <<  *argLength << " - Sprite's file name" << std::endl;

    std::cin >> i;
    std::cout << "Input new value" << std::endl;
    if(i < *argLength) {
        std::cin >> arg[i];
    }else{
        std::cin >> spriteName;
    }

    ifd.close();
    std::ofstream ofd(fname, std::ios::trunc | std::ios::binary);

    ofd.seekp(SEEK_SET);
    for (i = 0; i < *argLength; ++i) {
        ofd.write(reinterpret_cast<char *>(arg), sizeof(arg[0]) * *argLength);
    }

    writeFname(spriteName, ofd);

    ofd.close();
}

template<typename T>
void print(std::string &fname) {
    std::ifstream fd(fname, std::ios::binary);

    int i;
    T arg[*argLength];

    fd.seekg(SEEK_SET);
    if (fd.read(reinterpret_cast<char *>(arg), sizeof(arg[0]) * *argLength).eof()) {
        std::cout << "File empty" << std::endl;
        return;
    }

    i = 0;
    for (const auto &mes: *argNames) {
        std::cout << mes << ": " << arg[i++] << std::endl;
    }

    std::cout << "Sprite file name:" << readFname(fd) << std::endl;

    fd.close();
}

int main(){
    std::cout << "Write:" << std::endl << "\t" << codePlayer << " to work with players file" << std::endl;
    std::cout << "\t" << codeWeapon << " to work with weapons file" << std::endl;
    char c;
    std::cin >> c;

    switch(c){
        case codePlayer:
            argNames = &argPlayerNames;
            argLength = &argPlayerLength;
            break;
        case codeWeapon:
            argNames = &argWeaponNames;
            argLength = &argWeaponLength;
            break;
        default:
            std::cout << "Not supported" << std::endl;
            return 0;
    }

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