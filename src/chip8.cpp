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

    //copy fontset to memory
    for(int i=0;i<FONTSET_SIZE;i++)
    {
        memory[i] = fontSet[i];
    }
}

void Chip8::setMemory(int index, uint8_t value)
{
    assert(index >= 0 && index < MEMORY_SIZE);
    memory[index] = value;
}

uint8_t Chip8::getMemory(int index)
{
    assert(index >= 0 && index < MEMORY_SIZE);
    return memory[index];
}

void Chip8::setRegister(int index, uint8_t value)
{
    assert(index >=0 && index < 16);
    V[index] = value;
}

unsigned char Chip8::accessRegister(int index)
{
    assert(index >=0 && index < 16);
    return V[index];
}

void Chip8::push(uint16_t value)
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

uint16_t Chip8::pop()
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

uint16_t Chip8::getSP()
{
    return SP;
}

uint16_t Chip8::getStackTop()
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

void Chip8::draw(int x, int y, unsigned char* sprite, int size)
{
    /*
    size = 5
    "2"	   Binary	Hex
    ****  11110000  0xF0
        * 00000001  0x10
    ****  11110000  0xF0
    *     10000000  0x80
    ****  11110000  0xF0

         x1
         |
        \/
    y1-> 11110000
    */
    for(int y1 = 0;y1 < size;y1++)
    {
        for(int x1 = 0;x1 < 8;x1++) 
        {
            if((sprite[y1] & (/*0x80 128*/0b10000000 >> x1)) == 0)
                continue;
            
            
            display[(y1+y)][(x1+x)] = true;
        }
    }
}


void Chip8::setDelayTimer(uint8_t value)
{
    delayTimer = value;
}

uint8_t Chip8::getDelayTimer()
{
    return delayTimer;
}