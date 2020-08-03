#include <iostream>
#include <fstream>
#include "../include/CPU.hpp"
using namespace std;

int main(int argc, char **argv){
    ifstream game;
    game.open(argv[1], ios::in | ios::binary);
    if(!game.is_open()){
        cout << "Arquivo não pode ser aberto" << endl;
        exit(1);
    }
    CPU *Chip8 = new CPU();
    Chip8->LoadGame(&game);
    delete Chip8;
    game.close();
    return 0;
}