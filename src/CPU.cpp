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
    switch(Opcode){
        case 0x0000:
            switch(Instruction & 0x00FF){
                case 0xEE: //Retorno de subrotina
                    break;
                case 0xE0: //Limpa a tela
                    break;
            }
            break;
        case 0x1000: // 0x1NNN Pula para endereço NNN
            break;
        case 0x2000: // 0x2NNN Pulo para subrotina no endereço NNN
            break;
        case 0x3000: // 0x3XNN Pula a próxima instrução se Reg[X] == NN 
            break;
        case 0x4000: // 0x4XNN Pula a próxima instrução se Reg[X] != NN
            break;
        case 0x5000: // 0x5XY0 Pula a próxima instrução se Reg[X] == Reg[Y]
            break;
        case 0x6000: // 0x6XNN Reg[X] = NN
            break;
        case 0x7000: // 0x7XNN Reg[X] += NN sem alteração de carry
            break;
        case 0x8000:
            switch(Instruction & 0x000F){
                case 0x0:
                    break;
                case 0x1:
                    break;
                case 0x2:
                    break;
                case 0x3:
                    break;
                case 0x4:
                    break;
                case 0x5:
                    break;
                case 0x6:
                    break;
                case 0x7:
                    break;
                case 0xE:
                    break;
            }
            break;
        case 0x9000: // 9XY0 Pula a próxima instrução se Reg[X] != Reg[Y]
            break;
        case 0xA000: // ANNN I = NNN
            break;
        case 0xB000: // BNNN PC = Reg[0] + NNN
            break;
        case 0xC000: // CXNN Reg[X] = rand()%(255 + 1) & NN
            break;
        case 0xD000: // DXYN Desenhar
            break;
        case 0xE000: // KeyPresses
            switch(Instruction & 0x00FF){
                case 0x9E:
                    break;
                case 0xA1:
                    break;
            }
            break;
        case 0xF000:
            switch(Instruction & 0x00FF){
                case 0x9E:
                    break;
                case 0xA1:
                    break;
                case 0x07:
                    break;
                case 0x0A:
                    break;
                case 0x15:
                    break;
                case 0x18:
                    break;
                case 0x1E:
                    break;
                case 0x29:
                    break;
                case 0x33:
                    break;
                case 0x55:
                    break;
                case 0x65:
                    break;
            }
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