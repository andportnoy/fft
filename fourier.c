float complex *fourier_matrix_custom(
	const float *freq, /* array specifying desired frequencies in Hz */
	int m,             /* length of freq, number of rows in output matrix */
	int n,             /* number of samples in input signal, n columns */
	int srate          /* input signal sample rate in Hz */
)
{
	/* Suppose I want 60 Hz to resonate and we have 240 Hz at input. A 60
	 * Hz signal would peak every 4 samples? At k = 1 we are mapping the
	 * whole input to the unit circle, and so we are mapping n samples. So
	 * by increasing k we increase the frequency that is being highlighted
	 * because we wind the input more times around the circle.
	 * If we have the input at 1000 Hz, k=2 will wind two times,
	 * highlighting a signal that happens every 500 samples, that is at 2
	 * Hz. Increasing k increases the frequency we are highlighting.
	 * If we have a second at 48000 Hz, we need to wind twice to highlight
	 * 2 Hz. We wind 24000 times to highlight 24000 Hz.
	 * If we have half a second at 48000 Hz, we need to wind once to
	 * highlight 2 Hz.
	 * So if we have a second at frequency f and we have a target frequency
	 * t, it doesn't matter what f is. We need to wind once to highlight 1
	 * Hz, twice to highlight 2 Hz, three to highlight 3 Hz, etc.
	 * If we have a half second, we need to scale down the winding by that
	 * fraction (multiply by that fraction). So if we have 1024 samples at
	 * 48000 Hz, we have 1024/48000 of a second, and to get 1 Hz we wind
	 * 1024/48000, to get 60 Hz we wind 60*1024/48000, etc.
	 */
	float complex (*fm)[n] = malloc(m * sizeof *fm);
	for (int k=0; k<m; ++k) {
		float w = freq[k]*n/(float)srate;
		for (int j=0; j<n; ++j) {
			fm[k][j] = cexpf(-2*M_PI*I*w*j/n);
		}
	}
	return (float complex *)fm;
}

float complex *fourier_matrix(int n) {
	float complex (*fm)[n] = malloc(n*sizeof *fm);
	for (int k=0; k<n; ++k) /* every row corresponds to a given k */
	for (int j=0; j<n; ++j) {
		fm[k][j] = cexpf(-2*M_PI*I*k*j/n);
	}
	return (float complex *)fm;
}

float complex *inverse_fourier_matrix(int n) {
	float complex (*ifm)[n] = malloc(n*sizeof *ifm);
	for (int k=0; k<n; ++k) /* every row corresponds to a given k */
	for (int j=0; j<n; ++j) {
		ifm[k][j] = cexpf(2*M_PI*I*k*j/n)/n;
	}
	return (float complex *)ifm;
}

void dft(const float complex * restrict fm, const float * restrict x, float complex * restrict y, int m, int n) {
	float complex (*mat)[n] = (float complex (*)[])fm;
	for (int i=0; i<m; ++i) {
		y[i] = 0;
		for (int j=0; j<n; ++j) {
			y[i] += mat[i][j] * x[j];
		}
	}
}

void idft(const float complex * restrict ifm, const float complex * restrict x, float * restrict y, int m, int n) {
	float complex (*mat)[n] = (float complex (*)[])ifm;
	for (int i=0; i<m; ++i) {
		y[i] = 0;
		for (int j=0; j<n; ++j) {
			y[i] += mat[i][j] * x[j];
		}
	}
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
