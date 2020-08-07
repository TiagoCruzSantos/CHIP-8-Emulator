#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "../include/CPU.hpp"
#include "../include/Video.hpp"

using namespace std;
using namespace chrono_literals;
using namespace this_thread;

int main(int argc, char **argv){
    ifstream game;
    game.open(argv[1], ios::in | ios::binary);
    if(!game.is_open()){
        cout << "Arquivo não pode ser aberto" << endl;
        exit(1);
    }
    bool Running = true;
    Video* Screen = new Video(32, 64, 300, 16, 16);
    CPU *Chip8 = new CPU();
    SDL_Event Event;
    Chip8->LoadGame(&game);
    while(Running){
        Chip8->DoCycle();
        if(Chip8->getDrawFlag()){
            Screen->DrawScreen(Chip8->getDisplay());
            Chip8->UnsetDrawFlag();
        }
        while(SDL_PollEvent(&Event)){
            switch(Event.type){
                case SDL_QUIT:
                    Running = false;
                break;
                case SDL_KEYDOWN:{
                    for(int i = 0; i < 16; i++){
                        if(Event.key.keysym.sym == KeyMap[i]){
                            Chip8->KeyDown(i);
                        }
                    }
                    break;
                }
                case SDL_KEYUP:{
                    for(int i = 0; i < 16; i++){
                        if(Event.key.keysym.sym == KeyMap[i]){
                            Chip8->KeyUp(i);
                        }
                    }
                    break;
                }
            }
        }
        sleep_for(2ms);
    }
    delete Chip8;
    delete Screen;
    game.close();
    return 0;
}