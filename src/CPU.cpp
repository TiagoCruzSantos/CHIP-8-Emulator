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
        this->Memory[i] = CHIP8_FONTSET[i];
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

int CPU::DoCycle(){
    uint16_t Instruction = this->Memory[this->PC] << 8 | this->Memory[this->PC + 1]; // Pegar instrução da memória
    uint16_t Opcode = Instruction & 0xF000;
    switch(Opcode){
        case 0x0000:{
            switch(Instruction & 0x00FF){
                case 0xEE:{ //Retorno de subrotina
                    this->PC = this->StackCall.top();
                    this->StackCall.pop();
                    this->StackAmnt--;
                    break;
                }
                case 0xE0:{ //Limpa a tela
                    break;
                }
            }
            break;
        }
        case 0x1000:{ // 0x1NNN Pula para endereço NNN
            uint16_t JumpAdrss = Instruction & 0x0FFF;
            this->PC = JumpAdrss;
            break;
        }
        case 0x2000:{ // 0x2NNN Pulo para subrotina no endereço NNN
            uint16_t JumpAdrss = Instruction & 0x0FFF;
            if(this->StackAmnt == 15){
                cerr << "StackOverflow na posição " << this->PC << " de memória" << endl;
                return 1;
            }
            this->StackAmnt++;
            this->StackCall.push(this->PC + 2);
            this->PC = JumpAdrss;
            break;
        }
        case 0x3000:{ // 0x3XNN Pula a próxima instrução se Reg[X] == NN
            uint8_t Constant = Instruction & 0x00FF;
            uint8_t RegPos = (Instruction & 0x0F00) >> 8;
            if(Constant == this->Reg[RegPos]){
                this->PC += 4;
            }else{
                this->PC += 2;
            }
            break;
        }
        case 0x4000:{ // 0x4XNN Pula a próxima instrução se Reg[X] != NN
            uint8_t Constant = Instruction & 0x00FF;
            uint8_t RegPos = (Instruction & 0x0F00) >> 8;
            if(Constant != this->Reg[RegPos]){
                this->PC += 4;
            }else{
                this->PC += 2;
            }
            break;
        }
        case 0x5000:{ // 0x5XY0 Pula a próxima instrução se Reg[X] == Reg[Y]
            uint8_t RegPosX = (Instruction & 0x0F00) >> 8;
            uint8_t RegPosY = (Instruction & 0x00F0) >> 4;
            if(this->Reg[RegPosX] == this->Reg[RegPosY]){
                this->PC += 4;
            }else{
                this->PC += 2;
            }
            break;
        }
        case 0x6000:{ // 0x6XNN Reg[X] = NN
            uint8_t Constant = Instruction & 0x00FF;
            uint8_t RegPos = (Instruction & 0x0F00) >> 8;
            this->Reg[RegPos] = Constant;
            this->PC += 2;
            break;
        }
        case 0x7000:{ // 0x7XNN Reg[X] += NN sem alteração de carry
            uint8_t Constant = Instruction & 0x00FF;
            uint8_t RegPos = (Instruction & 0x0F00) >> 8;
            this->Reg[RegPos] += Constant;
            this->PC += 2;
            break;
        }
        case 0x8000:{ // Operações binária e matemática
            uint8_t RegPosX = (Instruction & 0x0F00) >> 8;
            uint8_t RegPosY = (Instruction & 0x00F0) >> 4;
            switch(Instruction & 0x000F){
                case 0x0:{  
                    this->Reg[RegPosX] = this->Reg[RegPosY];
                    this->PC += 2;
                    break;
                }
                case 0x1:{
                    this->Reg[RegPosX] = this->Reg[RegPosX] | this->Reg[RegPosY];
                    this->PC += 2;
                    break;
                }
                case 0x2:{
                    this->Reg[RegPosX] = this->Reg[RegPosX] & this->Reg[RegPosY];
                    this->PC += 2;
                    break;
                }
                case 0x3:{
                    this->Reg[RegPosX] = this->Reg[RegPosX] ^ this->Reg[RegPosY];
                    this->PC += 2;
                    break;
                }
                case 0x4:{
                    if((int) (this->Reg[RegPosX] + this->Reg[RegPosY]) > 255){
                        Reg[0xF] = 0x1;
                    }else{
                        Reg[0xF] = 0x0;
                    }
                    this->Reg[RegPosX] += this->Reg[RegPosY];
                    this->PC += 2;
                    break;
                }
                case 0x5:{
                    if(this->Reg[RegPosX] > this->Reg[RegPosY]){
                        this->Reg[0xF] = 1;
                    }else{
                        this->Reg[0xF] = 0;
                    }
                    this->Reg[RegPosX] -= this->Reg[RegPosY];
                    this->PC += 2;
                    break;
                }
                case 0x6:{
                    this->Reg[0xF] = this->Reg[RegPosX] & 0x1;
                    this->Reg[RegPosX] = this->Reg[RegPosX] >> 1;
                    this->PC += 2;
                    break;
                }
                case 0x7:{
                    if(this->Reg[RegPosX] < this->Reg[RegPosY]){
                        this->Reg[0xF] = 1;
                    }else{
                        this->Reg[0xF] = 0;
                    }
                    this->Reg[RegPosX] = this->Reg[RegPosY] - this->Reg[RegPosX];
                    this->PC += 2;
                    break;
                }
                case 0xE:{
                    this->Reg[0xF] = this->Reg[RegPosX] & 0x8000;
                    this->Reg[RegPosX] = this->Reg[RegPosX] << 1;
                    this->PC += 2;
                    break;
                }
            }
            break;
        }
        case 0x9000:{ // 9XY0 Pula a próxima instrução se Reg[X] != Reg[Y]
            uint8_t RegPosX = (Instruction & 0x0F00) >> 8;
            uint8_t RegPosY = (Instruction & 0x00F0) >> 4;
            if(this->Reg[RegPosX] != this->Reg[RegPosY]){
                this->PC += 4;
            }else{
                this->PC += 2;
            }
            break;
        }
        case 0xA000:{ // ANNN I = NNN
            uint8_t Constant = Instruction & 0x0FFF;
            this->I = Constant;
            this->PC += 2;
            break;
        }
        case 0xB000:{ // BNNN PC = Reg[0] + NNN
            uint8_t Constant = Instruction & 0x0FFF;
            this->PC = Reg[0] + Constant;
            break;
        }
        case 0xC000:{ // CXNN Reg[X] = rand()%(255 + 1) & NN
            uint8_t Constant = Instruction & 0x00FF;
            uint8_t RegPos = (Instruction & 0x0F00) >> 8;
            this->Reg[RegPos] = rand()%(255 + 1) & Constant;
            this->PC += 2;
            break;
        }
        case 0xD000:{ // DXYN Desenhar
            break;
        }
        case 0xE000:{ // KeyPresses
            switch(Instruction & 0x00FF){
                case 0x9E:{
                    break;
                }
                case 0xA1:{
                    break;
                }
            }
            break;
        }
        case 0xF000:{
            uint8_t RegPos = (Instruction & 0x0F00) >> 8;
            switch(Instruction & 0x00FF){
                case 0x07:{
                    this->Reg[RegPos] = this->DalayTimer;
                    this->PC += 2;
                    break;
                }
                case 0x0A:{ // Esperar por input
                    break;
                }
                case 0x15:{
                    this->DalayTimer = this->Reg[RegPos];
                    this->PC += 2;
                    break;
                }
                case 0x18:{
                    this->SoundTimer = this->Reg[RegPos];
                    this->PC += 2;
                    break;
                }
                case 0x1E:{
                    this->I += this->Reg[RegPos];
                    this->PC += 2;
                    break;
                }
                case 0x29:{
                    this->I = this->Memory[(this->Reg[RegPos] & 0x000F) * 5];
                    this->PC += 2;
                    break;
                }
                case 0x33:{
                    uint8_t Hundreds = this->Reg[RegPos] / 100;
                    uint8_t Tens = (this->Reg[RegPos] - Hundreds) / 10;
                    uint8_t Ones = (this->Reg[RegPos] - Hundreds - Tens);
                    this->Memory[this->I] = Hundreds;
                    this->Memory[this->I + 1] = Tens;
                    this->Memory[this->I + 2] = Ones;
                    this->PC += 2;
                    break;
                }
                case 0x55:{
                    for(int i = 0; i <= RegPos; i++){
                        this->Memory[this->I + i] = this->Reg[i];
                    }
                    this->PC += 2;
                    break;
                }
                case 0x65:{
                    for(int i = 0; i <= RegPos; i++){
                        this->Reg[i] = this->Memory[this->I + i];
                    }
                    this->PC += 2;
                    break;
                }
            }
            break;
        }
        default:
            cerr << "Instrução desconhecida: " << hex << Instruction << endl;
            return 2; 
            break;
    }
    return 0;
}

int CPU::LoadGame(ifstream *f){ //Carrega o jogo na memória a partir da posição 512/0x200
    int i = 0x200; 
    while(!(f->eof())){
        f->read((char *) (&(this->Memory[i])), 1);
        i++;
    }
    return 0;
}

bool CPU::getDrawFlag(){
    return this->DrawFlag;
}