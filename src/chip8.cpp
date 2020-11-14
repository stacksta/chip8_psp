#include "chip8.h"

void Chip8::setMemory(int index, unsigned char value)
{
    assert(index >= 0 && index < MEMORY_SIZE);
    memory[index] = value;
}

unsigned char Chip8::getMemory(int index)
{
    assert(index >= 0 && index < MEMORY_SIZE);
    return memory[index];
}

void Chip8::setRegister(int index, unsigned char value)
{
    assert(index >=0 && index < 16);
    V[index] = value;
}

unsigned char Chip8::accessRegister(int index)
{
    assert(index >=0 && index < 16);
    return V[index];
}