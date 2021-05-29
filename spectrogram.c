int main() {
	audio_initialize();
	int m = 160;
	float freq[m];
	for (int i=0; i<m; ++i)
		freq[i] = i*16;
	float complex *fm = fourier_matrix_custom(freq, m, NFRAMES, SRATE);
	float complex *y = malloc(m * sizeof *y);
	float *mag = malloc(m * sizeof *mag);
	for (;;) {
		patype *chunk = audio_record();
		dft(fm, chunk, y, m, NFRAMES);
		magnitude(y, mag, m);
#if 1
		for (int i=0; i<m; ++i) {
			//printf("%2.0f", mag[i]);
			printf(mag[i]>30? "#": " ");
		}
#else
		int imax = -1;
		float max = 0;
		for (int i=0; i<m; ++i) {
			if (mag[i] > max) {
				max = mag[i];
				imax = i;
			}
		}
		for (int i=0; i<m; ++i) {
			printf(i==imax? "#": " ");
		}
		printf("%6.2f Hz", freq[imax]);
#endif
		printf("\n");
	}
	audio_terminate();
}

