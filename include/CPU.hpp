#ifndef _CPU_
#define _CPU_
#include <iostream>
#include <fstream>
#include <stack>
#include <SDL2/SDL.h>
#include <stdlib.h>
using namespace std;

static uint8_t CHIP8_FONTSET[80] = { 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

static uint8_t KeyMap[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};

class CPU {
    private:
        uint8_t *Memory;
        uint8_t *Reg; //Registradores
        uint16_t I;
        uint16_t PC; //Program Couter
        uint8_t **Gfx;
        uint8_t DalayTimer;
        uint8_t SoundTimer;
        stack<uint16_t> StackCall;
        int StackAmnt;
        uint8_t *KeyPad;
        bool DrawFlag;

    public:
        CPU();
        ~CPU();
        bool getDrawFlag();
        int LoadGame(ifstream *f);
        int DoCycle();
        int UnsetDrawFlag();
        void DebugDraw();
        int KeyDown(int Index);
        int KeyUp(int Index);
        uint8_t **getDisplay();
};

#endif