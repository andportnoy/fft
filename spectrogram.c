int main() {
	audio_initialize();
	float complex *y = malloc(NFRAMES * sizeof *y);
	float *mag = malloc(NFRAMES * sizeof *mag);
	for (;;) {
		patype *chunk = audio_record();
		msdiff();
		fft(chunk, y, NFRAMES);
		double ms = msdiff();
		magnitude(y, mag, NFRAMES);
#if 1
		for (int i=0; i<160; ++i) {
			//printf("%2.0f", mag[i]);
			printf(mag[i]>10? "#": " ");
		}
#else
		int imax = -1;
		float max = 0;
		for (int i=0; i<160; ++i) {
			if (mag[i] > max) {
				max = mag[i];
				imax = i;
			}
		}
		for (int i=0; i<160; ++i) {
			printf(i==imax? "#": " ");
		}
#endif
		printf("%.2f ms", ms);
		printf("\n");
	}
	audio_terminate();
}

