#ifndef CHIP8_H
#define CHIP8_H

#include <assert.h>

const int MEMORY_SIZE = 4096;
const int WIDTH = 64;
const int HEIGHT = 32;
const int SCALE = 10;


const int FONTSET_SIZE = 80;
const unsigned char fontSet[FONTSET_SIZE] = 
{
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
    0xF0, 0x80, 0xF0, 0x80, 0x80, // F
};

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