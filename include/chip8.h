#ifndef CHIP8_H
#define CHIP8_H

#include <assert.h>

const int MEMORY_SIZE = 4096;

class Chip8 
{
    public:
        void setMemory(int index, unsigned char value);
        unsigned char getMemory(int index);

    private:
        unsigned char memory[MEMORY_SIZE];
};



#endif