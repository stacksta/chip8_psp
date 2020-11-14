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