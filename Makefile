CFLAGS += -std=gnu99 -O3 -Wall -Wextra -march=native -flto
CFLAGS += $(INCLUDE:%=-include %)

PROG=spectrogram record play compress fouriertest fourierbench
INCLUDE=common.h
all: $(PROG)

fourierbench fouriertest compress spectrogram: fourier.o
fourierbench fouriertest compress spectrogram: INCLUDE += fourier.h

$(PROG): audio.o utils.o
$(PROG): INCLUDE += audio.h utils.h
$(PROG): LDFLAGS += -lm -lportaudio
audio.o: INCLUDE += portaudio.h

clean:
	rm -rf $(PROG) *.o
