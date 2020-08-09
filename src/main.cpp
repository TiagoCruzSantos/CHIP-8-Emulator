#include <iostream>
#include <fstream>
#include "../include/CPU.hpp"
#include "../include/Video.hpp"

using namespace std;

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
                    if(Event.key.keysym.sym == SDLK_F1){
                        delete Chip8;
                        Chip8 = new CPU();
                        game.clear();
                        game.seekg(0);
                        Chip8->LoadGame(&game);
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
        if(Chip8->getPlaySound()){
            Chip8->UnsetPlaySound();
            SDL_PauseAudio(0);
            SDL_Delay(90);
            SDL_PauseAudio(1);
        }else{
            SDL_Delay(2);
        }
    }
    delete Chip8;
    delete Screen;
    game.close();
    return 0;
}