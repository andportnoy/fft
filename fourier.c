float complex *fourier_matrix(int n) {
	float complex (*fm)[n] = malloc(n*sizeof *fm);
	for (int k=0; k<n; ++k) /* every row corresponds to a given k */
	for (int j=0; j<n; ++j) {
		fm[k][j] = cexpf(-2*M_PI*I*k*j/n);
	}
	return (float complex *)fm;
}

void matvec(const float complex * restrict m, const float * restrict x, float complex * restrict y, int n) {
	float complex (*mat)[n] = (float complex (*)[])m;
	for (int i=0; i<n; ++i) {
		y[i] = 0;
		for (int j=0; j<n; ++j) {
			y[i] += mat[i][j] * x[j];
		}
	}
}

void dft(const float complex * restrict fm, const float * restrict x, float complex * restrict y, int n) {
	matvec(fm, x, y, n);
}

void magnitude(const float complex * restrict x, float * restrict y, int n) {
	for (int i=0; i<n; ++i)
		y[i] = cabsf(x[i]);
}

double msdiff(void) {
	static struct timespec cur, prev;
	static int initialized = 0;
	if (!initialized++) {
		clock_gettime(CLOCK_MONOTONIC, &prev);
		return 0;
	}
	clock_gettime(CLOCK_MONOTONIC, &cur);
	double ms = (cur.tv_sec - prev.tv_sec) * 1000
	          + (cur.tv_nsec - prev.tv_nsec) / 1e6;
	prev = cur;
	return ms;
}

int main() {
	audio_initialize();
	float complex *fm = fourier_matrix(NFRAMES);
	float complex *y = malloc(NFRAMES * sizeof *y);
	float *mag = malloc(NFRAMES * sizeof *mag);
	for (;;) {
		patype *chunk = audio_record();
		dft(fm, chunk, y, NFRAMES);
		magnitude(y, mag, NFRAMES);
		for (int i=0; i<80; ++i)
			printf(mag[i]>10? "#": " ");
		printf("\n");
	}
	audio_terminate();
}
