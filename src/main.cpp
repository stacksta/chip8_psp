#include <pspkernel.h>
#include <pspdebug.h>
#include <psptypes.h>
#include <time.h>
#include <psprtc.h>

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

	//fps
	float fps {};
	char fpsText[25] {};
	uint64_t tickNow {};
	uint64_t tickLast {};

	uint32_t tickResolution {};


	char debug[256] = "debug text";


	Chip8 chip8;
	//chip8.setMemory(10, 'A');

	//stack tests
	chip8.push('z');
	chip8.push('y');
	
	//auto val = chip8.getStackTop();

	uint16_t val1 = chip8.pop();
	auto temp1 = chip8.getSP();
	uint16_t val2 = chip8.pop();
	auto temp2 = chip8.getSP();

	//display test
	//chip8.setDisplay(0, 0, true);
	//display 420
	chip8.draw(0, 0, &chip8.memory[20], 5);
	chip8.draw(10, 0, &chip8.memory[10], 5);
	chip8.draw(20, 0, &chip8.memory[0], 5);

	//delay timer
	chip8.setDelayTimer(0xFF);
	chip8.setSoundTimer(0xFF);

	while(isRunning())
	{
		tickResolution = static_cast<float>(sceRtcGetTickResolution());

		g2dClear(BLACK);

		sprintf(debug,"chip8\n\npop: %c SP: %u\npop: %c SP: %u\n %c", val1, temp1, val2, temp2);
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

		fps++;
		sceRtcGetCurrentTick(&tickNow);

		if(((tickNow - tickLast)/((float)tickResolution)) >= 1.0f)
		{
			tickLast = tickNow;
			sprintf(fpsText,"FPS: %f",fps);
			fps = 0;

			if(chip8.getDelayTimer() > 0)
			{
				chip8.setDelayTimer(chip8.getDelayTimer() - 1);
				intraFontPrintf(font, 200, 200, "delay");
			}
			if(chip8.getSoundTimer() > 0)
			{
				chip8.setSoundTimer(chip8.getSoundTimer() - 1);
				intraFontPrintf(font, 200, 250, "sound");
			}

		}
		intraFontPrintf(font, 375, 20, fpsText);


		g2dFlip(G2D_VSYNC);

	}

	sceKernelExitGame();	

	return 0;
}