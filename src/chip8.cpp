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
    PC = START_ADDRESS;
    SP = 0;

    delayTimer = 0;
    soundTimer = 0;

    //copy fontset to memory
    for(int i=0;i<FONTSET_SIZE;i++)
    {
        memory[i] = fontSet[i];
    }

    //for random number generation
    time_t t;
    srand((unsigned)time(&t));
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

   //wrap around
    // uint8_t xPos = V[x] % WIDTH;
    // uint8_t yPos = V[y] % HEIGHT;

    V[0xF] = 0;

    for(int y1 = 0;y1 < size;y1++)
    {
        for(int x1 = 0;x1 < 8;x1++) 
        {
            if((sprite[y1] & (/*0x80 128*/0b10000000 >> x1)) == 0)
                continue;
            
            if(display[(y1+y) % HEIGHT][(x1+x) % WIDTH])
            {
                V[0xF] = 1;
            }
            
            display[(y1+y) % HEIGHT][(x1+x) % WIDTH] ^= true;
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


void Chip8::setSoundTimer(uint8_t value)
{
    soundTimer = value;
}

uint8_t Chip8::getSoundTimer()
{
    return soundTimer;
}

bool Chip8::loadRom(const char* filename)
{
    //open the rom as binary stream and move file pointer to the end
    std::ifstream rom(filename, std::ios::binary|std::ios::ate);

    if(rom.is_open())
    {
        //get the rom size
        std::streampos size = rom.tellg();
        char* buffer = new char[size];

        //go back to the beginning of the file and fill the buffer
        rom.seekg(0, std::ios::beg);
        rom.read(buffer, size);
        rom.close();

        //load roms contents to chip8's memory
        for(auto i=0;i<size;i++)
        {
            memory[START_ADDRESS + i] = buffer[i];
        }
        //free buffer
        delete[] buffer;
        return true;
    }
    return false;

}

uint16_t Chip8::getWord(int index)
{
    uint8_t byte1 = memory[index];
    uint8_t byte2 = memory[index+1];
    return byte1 << 8| byte2;
}

void Chip8::decodeAndDecode(uint16_t opcode)
{
    uint16_t nnn = opcode & 0x0fff;
    switch(opcode & 0xf000)
    {
        case 0x1000: 
                    // 1nnn: JP addr
                    //uint16_t nnn = opcode & 0x0fff;
                    PC = nnn;
                    break; 
        case 0x2000:
                    // 2nnn: CALL addr
                    push(PC);
                    PC = nnn;
                    break;

        case 0x3000:
                    // 3xkk: SE Vx, byte
                    {
                        uint8_t reg = (opcode & 0x0F00) >> 8;
                        uint8_t byte = (opcode & 0x0FF);
                        if(V[reg] == byte)
                            PC += 2;
                        break;
                    }

        case 0x4000:
                    // 4xkk: SNE Vx, byte
                    {
                        uint8_t reg = (opcode & 0x0F00) >> 8;
                        uint16_t byte = (opcode & 0x0FF);
                        if(V[reg] != byte)
                            PC += 2;
                        break;
                    }

        case 0x5000:
                    // 5xy0: SE Vx, Vy
                    {
                        uint8_t reg1 = (opcode & 0x0F00) >> 8;
                        uint8_t reg2 = (opcode & 0x00F0) >> 4;
                        if(V[reg1] == V[reg2])
                            PC += 2;
                        break;
                    }

        case 0x6000:
                    // 6xkk: LD Vx, byte (Vx = byte (kk))
                    {
                        uint8_t reg = (opcode & 0x0F00) >> 8;
                        uint16_t kk = opcode & 0x00FF;
                        V[reg] = kk;
                        break;
                    }

        case 0x7000:
                    // 7xkk: ADD Vx, byte (Vx = Vx + kk)
                    {
                        uint8_t reg = (opcode & 0x0F00) >> 8;
                        uint16_t kk = opcode & 0x00FF;
                        V[reg] = V[reg] + kk;
                    }
                    break;

        case 0x8000:
                    {
                        // 8xy_  _ -> z
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        uint8_t y = (opcode & 0x00F0) >> 4;
                        uint8_t z = (opcode & 0x000F);

                        // 8xy0: LD Vx, Vy
                        if(z == 0)
                        {
                            V[x] = V[y];
                        }
                        // 8xy1: OR Vx, Vy
                        else if(z == 1)
                        {
                            V[x] = V[x] | V[y];
                        }
                        // 8xy2: AND Vx, Vy
                        else if(z == 2)
                        {
                            V[x] = V[x] & V[y];
                        }
                        // 8xy3: XOR Vx, Vy
                        else if(z == 3)
                        {
                            V[x] = V[x] ^ V[y];
                        }
                        // 8xy4: ADD Vx, Vy
                        else if(z == 4)
                        {
                            V[x] = V[x] + V[y];
                            if(V[x] > 0xFF) // > 255
                                V[0xF] = 1;
                            else 
                                V[0xF] = 0;
                            //only the lowest 8bits of the result are kept,
                            V[x] = V[x] & 0x00FF;
                        }
                        // 8xy5: SUB Vx, Vy
                        else if(z == 5)
                        {
                            if(V[x] > V[y])
                                V[0xF] = 1;
                            else 
                                V[0xF] = 0;
                            V[x] = V[x] - V[y];
                        }
                        // 8xy6: SHR Vx {, Vy}
                        else if(z == 6)
                        {
                            uint8_t lsb = opcode & 0x000F;
                            if(lsb == 1)
                                V[0xF] = 1;
                            else 
                                V[0xF] = 0;
                            V[x] = V[x] / 2;
                        }
                        // 8xy7: SUBN Vx, Vy
                        else if(z == 7)
                        {
                            if(V[y] > V[x])
                                V[0xF] = 1;
                            else 
                                V[0xF] = 0;
                            V[x] = V[y] - V[x];
                        }
                        // 8xyE: SHL Vx {, Vy}
                        else if(z == 0xE)
                        {
                            uint8_t msb = x;//;(opcode & 0x0F00) >> 8;
                            if(msb == 1)
                                V[0xF] = 1;
                            else 
                                V[0xF] = 0;
                            V[x] = V[x] * 2;
                        }
                    }
                    break;


        case 0x9000:
                    // 9xy0: SNE Vx, Vy
                    {
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        uint8_t y = (opcode & 0x00F0) >> 4;
                        
                        if(V[x] != V[y])
                            PC += 2;
             
                    }
                    break;
        
        case 0xA000:
                    // Annn: LD I, addr (I = nnn)
                    {
                        uint16_t nnn = opcode & 0x0FFF;
                        I = nnn;
                    }
                    break;

        case 0xB000:
                    // Bnnn: JP V0, addr (jmp nnn + 40)
                    {
                        uint16_t nnn = opcode & 0xFFF;
                        PC += nnn + V[0];
                    }
                    break;
        
        case 0xC000:
                    // RND Vx, byte
                    {
                        //generate random number between 0-255
                        uint8_t random = rand() % 256;
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        uint8_t kk = opcode & 0x00FF;
                        V[x] = random & kk;
                    }
                    break;

        case 0xD000:
                    // Dxyn: DRW Vx, Vy nibble
                    {
                        uint8_t x = (opcode & 0xF00) >> 8;
                        uint8_t y = (opcode & 0xF0) >> 4;
                        uint8_t size = opcode & 0x000F;

                        draw(V[x], V[y], &memory[I], size);

                    }
                    break; 
        
        case 0xE000:
                    {
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        //TODO
                    }
                    break;

        case 0xF000:
                    {
                       uint8_t x = (opcode & 0x0F00) >> 8;
                       uint8_t byte = opcode & 0x00FF;

                        // Fx07 - LD Vx, DT
                       if(byte == 0x07)
                       {
                           V[x] = delayTimer;
                       }
                       // Fx0A - LD Vx, K
                       else if(byte == 0x0A)
                       {
                           //TODO
                       }
                       // Fx15 - LD DT, Vx
                       else if(byte == 0x15)
                       {
                           delayTimer = V[x];
                       }
                       // Fx18 - LD ST, Vx
                       else if(byte == 0x18)
                       {
                           soundTimer = V[x];
                       }
                       // Fx1E - ADD I, Vx
                       else if(byte == 0x1E)
                       {
                           //set VF to 1 when range overflow if
                           if(I + V[x] > 0xFFF)
                                V[0xF] = 1;
                           else 
                                V[0xF] = 0;
                            I += V[x];
                       }
                       // Fx29 - LD F, Vx
                       else if(byte == 0x29)
                       {
                           I = V[x] * 0x5;
                       }
                       // Fx33 - LD B, Vx
                       else if(byte == 0x33)
                       {
                           //bcd
                           //extract the digits
                            memory[I + 2] = V[x] % 10;
                            V[x] = V[x] / 10;

                            memory[I + 1] = V[x] % 10;
                            V[x] = V[x] / 10;

                            memory[I] = V[x] % 10;
                       }
                       // Fx55 - LD [I], Vx
                       else if(byte == 0x55)
                       {
                           for(uint8_t i = 0;i <= x;i++)
                           {
                               memory[I + i] = V[i];
                           }
                           I += x + 1;// when the operation is done, I = I + X + 1
                       }
                       // Fx65 - LD Vx, [I]
                       else if(byte == 0x65)
                       {
                           for(uint8_t i=0;i <= x; i++)
                           {
                               V[i] = memory[I + i];
                           }
                           I += x + 1;// when the operation is done, I = I + X + 1
                       }
                    }
                    break;
    }
}

void Chip8::execute(uint16_t opcode)
{
    switch(opcode)
    {
        case 0x00E0:
                    //clear screen
                    memset(display, 0, sizeof(display));
                    break;
        
        case 0x00EE:
                    //RET
                    PC = pop();
                    break;

        default:
            decodeAndDecode(opcode);
    }
}