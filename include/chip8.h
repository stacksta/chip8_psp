#ifndef CHIP8_H
#define CHIP8_H

#include <assert.h>
#include <cstdint>

const int MEMORY_SIZE = 4096;
const int WIDTH = 64;
const int HEIGHT = 32;
const int SCALE = 10;


const int FONTSET_SIZE = 80;
const uint8_t fontSet[FONTSET_SIZE] = 
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

        void setMemory(int index, uint8_t value);
        uint8_t getMemory(int index);

        void setRegister(int index, uint8_t value);
        uint8_t accessRegister(int index);

        //stack functions
        void push(uint16_t value);
        uint16_t  pop();

        uint16_t  getSP();

        uint16_t  getStackTop();

        //display functions
        void setDisplay(int x, int y, bool value);
        bool getDisplay(int x, int y);
        void draw(int x, int y, uint8_t* sprite, int size);// Dxyn - DRW Vx, Vy, nibble

        uint8_t memory[MEMORY_SIZE] {};

        //timer
        void setDelayTimer(uint8_t value);
        uint8_t getDelayTimer();

        void setSoundTimer(uint8_t value);
        uint8_t getSoundTimer();

    private:
        //unsigned char memory[MEMORY_SIZE] {};
        uint8_t V[16] {};

        uint16_t  I{};// 16 bit register

        uint16_t PC {};// program counter

        uint16_t SP {};// stack pointer

        uint8_t delayTimer {};
        uint8_t soundTimer {};

        uint16_t stack[16] {};

        bool display[HEIGHT][WIDTH] {};

};



#endif