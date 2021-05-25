#include <portaudio.h>

#define PAFORMAT paFloat32
#define SRATE 48000
#define NFRAMES 1024

#define pacheck(err, msg) { \
	PaError e = err; \
	if ((e) != paNoError) { \
		fprintf(stderr, "%s: %s\n", msg, Pa_GetErrorText(e)); \
		exit(EXIT_FAILURE); \
	} \
}

#define pawarn(err, msg) { \
	PaError e = err; \
	if ((e) != paNoError) { \
		fprintf(stderr, "%s: %s\n", msg, Pa_GetErrorText(e)); \
	} \
}

typedef float patype;

void audio_initialize(void);
void audio_terminate(void);
patype *audio_record(void);
void audio_play(patype *data);
