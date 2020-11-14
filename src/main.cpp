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

	//stack tests
	chip8.push('z');
	chip8.push('y');
	
	auto val = chip8.getStackTop();

	unsigned short val1 = chip8.pop();
	auto temp1 = chip8.getSP();
	unsigned short val2 = chip8.pop();
	auto temp2 = chip8.getSP();

	while(isRunning())
	{
		pspDebugScreenSetXY(0, 0);
		printf("chip8 %c\n\npop: %c SP: %u\npop: %c SP: %u\n %c", chip8.getMemory(10), val1, temp1, val2, temp2, val);
		sceDisplayWaitVblankStart();
	}

	sceKernelExitGame();	

	return 0;
}