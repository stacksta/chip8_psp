#include <pspkernel.h>
#include <pspdebug.h>
#include <psptypes.h>
#include <time.h>
#include <psprtc.h>

#include <pspgu.h>
#include <pspgum.h>

#include <pspaudiolib.h>
#include <pspaudio.h>

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
	//pspDebugScreenInit();
	setupExitCallback();

	g2dInit();

	intraFontInit();
	intraFont* font;
	font = intraFontLoad("flash0:/font/ltn8.pgf",0);

	//audio
	unsigned int audioData[256];
	memset(audioData, 100, sizeof(audioData));
	int channel = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, sizeof(audioData), PSP_AUDIO_FORMAT_STEREO);
	sceAudioSetChannelDataLen(channel, sizeof(audioData));
	sceAudioChangeChannelVolume(channel, PSP_VOLUME_MAX, PSP_VOLUME_MAX);

	//fps
	float fps {};
	char fpsText[25] {};
	uint64_t tickNow {};
	uint64_t tickLast {};

	uint32_t tickResolution {};


	//char debug[256] = "debug text";


	Chip8 chip8;
	bool rom = chip8.loadRom("./roms/PONG");

	while(isRunning())
	{
		tickResolution = static_cast<float>(sceRtcGetTickResolution());

		g2dClear(0x532B1D);//0x1D2B53

		//sprintf(debug,"chip8\n\npop: %c SP: %u\npop: %c SP: %u\n %c", val1, temp1, val2, temp2);
		// sprintf(debug,"loaded");
		// intraFontSetStyle(font, 1.f, WHITE,0, 0, INTRAFONT_ALIGN_LEFT);
		// if(rom)
		// 	intraFontPrintf(font, 170, 100, debug);



		for(int x = 0; x < WIDTH; x++)
		{
			for(int y = 0;y < HEIGHT; y++)
			{
				if(chip8.getDisplay(x, y))
				{
					g2dBeginRects(NULL);
					g2dSetColor(WHITE);
					g2dSetCoordXY((x + 5)* SCALE, (y + 12) * SCALE);
					g2dSetScaleWH(SCALE, SCALE);
					g2dAdd();
					g2dEnd();
				}
				else 
				{
					g2dBeginRects(NULL);
					g2dSetColor(BLACK);
					g2dSetCoordXY((x + 5) * SCALE, (y + 12) * SCALE);
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
		}

			if(chip8.getDelayTimer() > 0)
			{
				chip8.setDelayTimer(chip8.getDelayTimer() - 1);
				//intraFontPrintf(font, 200, 200, "delay");
			}
			if(chip8.getSoundTimer() > 0)
			{
				chip8.setSoundTimer(chip8.getSoundTimer() - 1);
				//intraFontPrintf(font, 200, 250, "sound");
				sceAudioOutput(channel, PSP_VOLUME_MAX, audioData);

			}

		//}
		uint16_t opcode = chip8.getWord(chip8.PC);		
		chip8.PC += 2;
		chip8.execute(opcode);

		intraFontSetStyle(font, 1.f, WHITE,0, 0, INTRAFONT_ALIGN_LEFT);
		intraFontPrintf(font, 375, 20, fpsText);

		intraFontSetStyle(font, 1.f, WHITE,0, 0, INTRAFONT_ALIGN_LEFT);
		intraFontPrintf(font, 120, 240, "Chip8 Emulator");

		g2dFlip(G2D_VSYNC);

	}

	sceKernelExitGame();	

	return 0;
}