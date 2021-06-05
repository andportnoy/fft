int main(void) {
	int n = 16*NFRAMES;
	/* allocate and randomly initialize data */
	float *x = malloc(n * sizeof*x);
	//float complex *fm = fourier_matrix(n);
	float complex *y = malloc(n * sizeof*y);
	for (;;) {
		for (int i=0; i<n; ++i)
			x[i] = (float)rand()/RAND_MAX;
		msdiff();
#if 0
		dft(fm, x, y, n, n);
#else
		fft(x, y, n);
#endif
		printf("%.3f ms\n", msdiff());
	}
}
