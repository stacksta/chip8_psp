TARGET = chip8

OBJS = ./src/chip8.o ./src/main.o ./src/callback.o

INCDIR += ./include/
CFLAGS = -G0 -Wall -Os
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS = 
LIBS= -lstdc++ -lpsprtc -lpspsdk


EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = chip8
PSP_FW_VERSION=500

PSP_LARGE_MEMORY = 0
BUILD_PRX = 1

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
