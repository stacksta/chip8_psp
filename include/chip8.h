#ifndef CHIP8_H
#define CHIP8_H

#include <assert.h>

const int MEMORY_SIZE = 4096;
const int WIDTH = 64;
const int HEIGHT = 32;
const int SCALE = 10;

class Chip8 
{
    public:
        Chip8();

        void reset();

        void setMemory(int index, unsigned char value);
        unsigned char getMemory(int index);

        void setRegister(int index, unsigned char value);
        unsigned char accessRegister(int index);

        //stack functions
        void push(unsigned short value);
        unsigned short pop();

        unsigned short getSP();

        unsigned short getStackTop();

        //display functions
        void setDisplay(int x, int y, bool value);
        bool getDisplay(int x, int y);

    private:
        unsigned char memory[MEMORY_SIZE] {};
        unsigned char V[16] {};

        unsigned short I{};// 16 bit register

        unsigned short PC {};// program counter

        unsigned short SP {};// stack pointer

        unsigned char delayTimer {};
        unsigned char soundTimer {};

        unsigned short stack[16] {};

        bool display[HEIGHT][WIDTH] {};

};



#endif