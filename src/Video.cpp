#include "../include/Video.hpp"
#include <iostream>
using namespace std;

Video::Video(int Height, int Width, int Dalay, int PixelH, int PixelW){
    this->Height = Height;
    this->Width = Width;
    this->Dalay = Dalay;
    this->PixelH = PixelH;
    this->PixelW = PixelW;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        cerr << "SDL não pode ser iniciado" << endl;
        return;
    }
    this->Window = SDL_CreateWindow("ChipOctle", 100, 100, Width * PixelW, Height * PixelH, 0);
    this->Renderer = SDL_CreateRenderer(this->Window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(this->Renderer, 0, 0, 0, 255);
    SDL_RenderPresent(this->Renderer);
    this->AudioSpec.freq = 22000;
    this->AudioSpec.format = AUDIO_S16SYS;
    this->AudioSpec.channels = 1;
    this->AudioSpec.samples = 1024;
    this->AudioSpec.callback = Video::AudioCallback;
    this->AudioSpec.userdata = NULL;
    SDL_OpenAudio(&(this->AudioSpec), NULL);
}

void Video::AudioCallback(void *UserData, Uint8 *Buffer, int Len){
    for(int i = 0; i < Len; i++){
        Buffer[i] = i;
    }
}

Video::~Video(){
    SDL_CloseAudio();
    SDL_Quit();
}

int Video::DrawPixel(int CordX, int CordY){
    SDL_Rect Pixel = {CordX * PixelW, CordY * PixelH, this->PixelW, this->PixelH};
    SDL_RenderFillRect(this->Renderer, &Pixel);
}

int Video::DrawScreen(uint8_t **Display){
    SDL_RenderClear(this->Renderer);
    SDL_SetRenderDrawColor(this->Renderer, 255, 255, 255, 255);
    for(int i = 0; i < this->Height; i++){
        for(int k = 0; k < this->Width; k++){
            if(Display[k][i]){
                this->DrawPixel(k, i);
            }
        }
    }
    SDL_SetRenderDrawColor(this->Renderer, 0, 0, 0, 255);
    SDL_RenderPresent(this->Renderer);
}