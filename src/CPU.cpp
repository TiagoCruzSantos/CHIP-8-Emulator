#include "../include/CPU.hpp"
#include <bitset>
#define HEIGHT 32
#define WIDHT 64

CPU::CPU(){
    this->PC = 0x200; //Começar na posição 0x200 da memória
    this->I = 0;
    this->DalayTimer = 0;
    this->SoundTimer = 0;
    this->StackAmnt = 0;
    this->PlaySound = false;

    this->Memory = (uint8_t *) calloc(4096, sizeof(uint8_t));
    this->Reg = (uint8_t *) calloc(16, sizeof(uint8_t));
    this->KeyPad = (uint8_t *) calloc(16, sizeof(uint8_t));
    this->Gfx = (uint8_t **) malloc(WIDHT * sizeof(uint8_t*));
    for(int i = 0; i < WIDHT; i++){
        this->Gfx[i] = (uint8_t *) calloc(HEIGHT, sizeof(uint8_t));
    }
    // Carregar fontset na memória
    for(int i = 0; i < 80; i++){
        this->Memory[i] = CHIP8_FONTSET[i];
        //cout << hex << (int) CHIP8_FONTSET[i] << endl;
    }
    srand(time(NULL));
    this->DrawFlag = true;
}

CPU::~CPU(){
    free(this->Memory);
    free(this->Reg);
    free(this->KeyPad);
    for(int i = 0; i < WIDHT; i++){
        free(this->Gfx[i]);
    }
    free(this->Gfx);
}

int CPU::DoCycle(){
    uint16_t Instruction = this->Memory[this->PC] << 8 | this->Memory[this->PC + 1]; // Pegar instrução da memória
    uint16_t Opcode = Instruction & 0xF000;
    //cout << "Instrução: " << hex << Instruction;
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
                    for(int i = 0; i < WIDHT; i++){
                        for(int k = 0; k < HEIGHT; k++){
                            this->Gfx[i][k] = 0x0;
                        }
                    }
                    this->DrawFlag = true;
                    this->PC += 2;
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
            uint16_t Constant = Instruction & 0x0FFF;
            this->I = Constant;
            this->PC += 2;
            break;
        }
        case 0xB000:{ // BNNN PC = Reg[0] + NNN
            uint16_t Constant = Instruction & 0x0FFF;
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
            uint8_t RegPosX = (Instruction & 0x0F00) >> 8;
            uint8_t RegPosY = (Instruction & 0x00F0) >> 4;
            uint8_t CordX = this->Reg[RegPosX];
            uint8_t CordY = this->Reg[RegPosY];
            uint8_t Height = Instruction & 0x000F;
            uint8_t Row;
            this->Reg[0xF] = 0;
            //cout << "\n";
            for(int i = 0; i < Height; i++){
                Row = this->Memory[this->I + i];
                //cout << '|' << bitset<8>(Row) << "|\n";
                for(int k = 0; k < 8; k++){
                    if((Row & (0x80 >> k)) != 0){
                        if(this->Gfx[(CordX + k)%WIDHT][(CordY + i)%HEIGHT] == 1){
                            this->Reg[0xF] = 1;
                        } 
                        this->Gfx[(CordX + k)%WIDHT][(CordY + i)%HEIGHT] ^= 1;
                    }
                }
            }
            this->DrawFlag = true;
            this->PC += 2;
            break;
        }
        case 0xE000:{ // KeyPresses
            switch(Instruction & 0x00FF){
                case 0x9E:{
                    if(this->KeyPad[this->Reg[(Instruction & 0x0F00) >> 8]] != 0){
                        this->PC += 4;
                    }else{
                        this->PC += 2;
                    }
                    break;
                }
                case 0xA1:{
                    if(this->KeyPad[this->Reg[(Instruction & 0x0F00) >> 8]] == 0){
                        this->PC += 4;
                    }else{
                        this->PC += 2;
                    }
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
                    bool KeyPress = false;
                    for(int i = 0; i < 16; i++){
                        if(this->KeyPad[i] != 0)
                        {
                            this->Reg[(Instruction & 0x0F00) >> 8] = i;
                            KeyPress = true;
                        }
                    }
                    if(!KeyPress){
                        return 3;
                    }
                    this->PC += 2;
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
                    this->I = (this->Reg[RegPos] & 0x000F) * 5;
                    this->PC += 2;
                    break;
                }
                case 0x33:{
                    uint8_t Hundreds = this->Reg[RegPos] / 100;
                    uint8_t Tens = (this->Reg[RegPos] / 10) % 10;
                    uint8_t Ones = this->Reg[RegPos] % 10;
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
    //cout << " DONE" << endl;
    if(this->DalayTimer > 0){
        this->DalayTimer--;
    }
    if(this->SoundTimer > 0){
        if(this->SoundTimer == 1){
            this->PlaySound = true;
        }
        this->SoundTimer--;
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

void CPU::DebugDraw(){
    for(int i = 0; i < HEIGHT; i++){
        cout << '|';
        for(int k = 0; k < WIDHT; k++){
            if(this->Gfx[k][i] == 1){
                cout << '0';
            }else{
                cout << ' ';
            }
        }
        cout << '|';
        cout << endl;
    }
    this->DrawFlag = false;
}

int CPU::UnsetDrawFlag(){
    this->DrawFlag = false;
    return 0;
}

uint8_t **CPU::getDisplay(){
    return this->Gfx;
}

int CPU::KeyDown(int Index){
    this->KeyPad[Index] = 1;
    return 0;
}

int CPU::KeyUp(int Index){
    this->KeyPad[Index] = 0;
    return 0;
}

bool CPU::getPlaySound(){
    return this->PlaySound;
}
void CPU::UnsetPlaySound(){
    this->PlaySound = false;
}