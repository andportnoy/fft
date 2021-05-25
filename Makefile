CFLAGS += -std=gnu99 -O3 -Wall -Wextra -march=native -flto
CFLAGS += $(INCLUDE:%=-include %)

PROG=fourier
INCLUDE=common.h
all: $(PROG)

fourier: audio.o
fourier: INCLUDE += audio.h
fourier: LDFLAGS += -lm
audio.o: INCLUDe += portaudio.h

clean:
	rm -rf $(PROG) audio.o
