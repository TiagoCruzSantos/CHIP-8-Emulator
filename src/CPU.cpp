#include "../include/CPU.hpp"

CPU::CPU(){
    this->PC = 0x200; //Começar na posição 0x200 da memória
    this->I = 0;
    this->DalayTimer = 0;
    this->SoundTimer = 0;
    this->StackAmnt = 0;

    this->Memory = (uint8_t *) calloc(4096, sizeof(uint8_t));
    this->Reg = (uint8_t *) calloc(16, sizeof(uint8_t));
    this->Gfx = (uint8_t **) malloc(64 * sizeof(uint8_t*));
    for(int i = 0; i < 64; i++){
        this->Gfx[i] = (uint8_t *) calloc(32, sizeof(uint8_t));
    }
    // Carregar fontset na memória
    for(int i = 0; i < 80; i++){
        this->Memory[i + 0x50] = CHIP8_FONTSET[i];
    }
}

CPU::~CPU(){
    free(this->Memory);
    free(this->Reg);
    for(int i = 0; i < 64; i++){
        free(this->Gfx[i]);
    }
    free(this->Gfx);
}

void CPU::DoCycle(){
    uint16_t Instruction = this->Memory[this->PC] << 8 | this->Memory[this->PC + 1]; // Pegar instrução da memória
    uint16_t Opcode = Instruction & 0xF000;
    switch(Opcode >> 8){
        case 0x00:
            switch(Opcode & 0x00FF){
                case 0xEE: //Retorno de subrotina
                    break;
                case 0xE0: //Limpa a tela
                    break;
            }
            break;
        case 0x10: // Pula para endereço NNN
            break;
        case 0x20: // Pulo para subrotina
            break;
        case 0x30:
            break;
        case 0x40:
            break;
        case 0x50:
            break;
        case 0x60:
            break;
        case 0x70:
            break;
        case 0x80:
            break;
        case 0x90:
            break;
        case 0xA0:
            break;
        case 0xB0:
            break;
        case 0xC0:
            break;
        case 0xD0:
            break;
        case 0xE0:
            break;
        case 0xF0:
            break;
    }
}

void CPU::LoadGame(ifstream *f){ //Carrega o jogo na memória a partir da posição 512/0x200
    int i = 0x200; 
    while(!(f->eof())){
        f->read((char *) (&(this->Memory[i])), 1);
        i++;
    }
}

bool CPU::getDrawFlag(){
    return this->DrawFlag;
}