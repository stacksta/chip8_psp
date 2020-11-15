#include "chip8.h"

Chip8::Chip8()
{
    reset();
}

void Chip8::reset()
{
    for(int i=0;i<16;i++)
    {
        V[i] = 0;
        stack[i] = 0;
    }
    I = 0;
    PC = 0;
    SP = 0;

    delayTimer = 0;
    soundTimer = 0;
}

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

void Chip8::push(unsigned short value)
{
    /*  CALL addr
        increment SP,
        put PC on stack top,
        PC = addr
    */
    SP += 1;
    assert(SP >=0 && SP < 16);
    stack[SP] = value;//PC;
   // PC = value;
}

unsigned short Chip8::pop()
{
    /* RET
        PC = stack top,
        SP - 1
    */
  // PC = stack[SP];
   unsigned short value = stack[SP];
   SP -= 1;
   assert(SP >=0 && SP < 16);
   return value; 
}

unsigned short Chip8::getSP()
{
    return SP;
}

unsigned short Chip8::getStackTop()
{
    return stack[SP];
}


void Chip8::setDisplay(int x, int y, bool value)
{
    display[y][x] = value;
}

bool Chip8::getDisplay(int x, int y)
{
    return display[y][x];
}
