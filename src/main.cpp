#include <pspkernel.h>
#include <pspdebug.h>

#include <pspgu.h>
#include <pspgum.h>

#include <cstdio>

#include "callback.h"
#include "intraFont.h"
#include "glib2d.h"

#include "chip8.h"

#define VERS 1 
#define REVS 0

PSP_MODULE_INFO("chip8", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER); 
PSP_HEAP_SIZE_MAX();

int main(void) 
{       
	pspDebugScreenInit();
	setupExitCallback();

	g2dInit();

	intraFontInit();
	intraFont* font;
	font = intraFontLoad("flash0:/font/ltn8.pgf",0);

	char debug[256] = "debug text";


	Chip8 chip8;
	chip8.setMemory(10, 'A');

	//stack tests
	chip8.push('z');
	chip8.push('y');
	
	auto val = chip8.getStackTop();

	unsigned short val1 = chip8.pop();
	auto temp1 = chip8.getSP();
	unsigned short val2 = chip8.pop();
	auto temp2 = chip8.getSP();

	//display test
	chip8.setDisplay(0, 0, true);

	while(isRunning())
	{
		g2dClear(BLACK);

		sprintf(debug,"chip8 %c\n\npop: %c SP: %u\npop: %c SP: %u\n %c", chip8.getMemory(10), val1, temp1, val2, temp2, val);
		intraFontSetStyle(font, 1.f, WHITE,0, 0, INTRAFONT_ALIGN_LEFT);
		intraFontPrintf(font, 170, 100, debug);

		for(int x = 0; x < WIDTH; x++)
		{
			for(int y = 0;y < HEIGHT; y++)
			{
				if(chip8.getDisplay(x, y))
				{
					g2dBeginRects(NULL);
					g2dSetColor(WHITE);
					g2dSetCoordXY(x * SCALE, y * SCALE);
					g2dSetScaleWH(SCALE, SCALE);
					g2dAdd();
					g2dEnd();
				}
			}
		}

		g2dFlip(G2D_VSYNC);

	}

	sceKernelExitGame();	

	return 0;
}