#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>

#include "callback.h"

#include "chip8.h"

#define VERS 1 
#define REVS 0

PSP_MODULE_INFO("chip8", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER); 
PSP_HEAP_SIZE_MAX();

#define printf pspDebugScreenPrintf

int main(void) 
{       
	pspDebugScreenInit();
	setupExitCallback();

	Chip8 chip8;
	chip8.setMemory(10, 'A');

	while(isRunning())
	{
		pspDebugScreenSetXY(0, 0);
		printf("chip8 %c", chip8.getMemory(10));
		sceDisplayWaitVblankStart();
	}

	sceKernelExitGame();	

	return 0;
}