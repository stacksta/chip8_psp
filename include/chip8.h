#ifndef CHIP8_H
#define CHIP8_H

#include <assert.h>

const int MEMORY_SIZE = 4096;

class Chip8 
{
    public:
        void setMemory(int index, unsigned char value);
        unsigned char getMemory(int index);

        void setRegister(int index, unsigned char value);
        unsigned char accessRegister(int index);

    private:
        unsigned char memory[MEMORY_SIZE] {};
        unsigned char V[16] {};

        unsigned short I{};// 16 bit register

        unsigned char delayTimer {};
        unsigned char soundTimer {};

        unsigned short PC;// program counter

        unsigned char SP;// stack pointer
};



#endif