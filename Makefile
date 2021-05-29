CFLAGS += -std=gnu99 -O3 -Wall -Wextra -march=native -flto
CFLAGS += $(INCLUDE:%=-include %)

PROG=spectrogram record play compress fouriertest
INCLUDE=common.h
all: $(PROG)

fouriertest compress spectrogram: fourier.o
fouriertest compress spectrogram: INCLUDE += fourier.h

$(PROG): audio.o utils.o
$(PROG): INCLUDE += audio.h utils.h
$(PROG): LDFLAGS += -lm -lportaudio
audio.o: INCLUDE += portaudio.h

clean:
	rm -rf $(PROG) *.o
