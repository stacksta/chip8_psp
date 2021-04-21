TARGET = chip8
GFX = ./include/gLib2D/glib2d.o
OBJS = ${GFX} ./src/chip8.o ./src/main.o ./src/callback.o

INCDIR += ./include/ ./include/gLib2D/ ./include/intraFont/
CFLAGS = -G0 -Wall -O3
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR += ./libs/
LDFLAGS = 
LIBS= -lstdc++ -lintrafont -lfreetype -lpng -ljpeg -lg -lGL -lGLU -lglut -lz -lm \
        -lpspvram -lpspaudio -lpspaudiocodec \
        -L${PSPDEV}/psp/sdk/lib \
        -L${PSPDEV}/psp \
        -lpspdebug  -lpspgum -lpspgu -lpspctrl -lpspge -lpsppower -lpsppower_driver \
        -lpspdisplay -lpsphprm  -lpspsdk -lpsprtc -lpsputility \
        -lpspnet_inet  -lc -lpspuser


EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = chip8
PSP_FW_VERSION=500

PSP_LARGE_MEMORY = 0
BUILD_PRX = 1

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
