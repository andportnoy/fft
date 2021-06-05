int main(void) {
	int n = 32;

	float complex *fm = fourier_matrix(n);

	float *input = malloc(n * sizeof*input);
	float complex *output_naive = malloc(n * sizeof*output_naive);
	float complex *output_fft = malloc(n * sizeof*output_fft);

	for (int i=0; i<n; ++i)
		input[i] = (float)rand()/RAND_MAX;

	dft(fm, input, output_naive, n, n);
	fft(input, output_fft, n);

	printf("naive:");
	for (int i=0; i<n; ++i)
		printf(" %+6.2f%+6.2f",
		  crealf(output_naive[i]), cimagf(output_naive[i]));
	printf("\n");
	printf("fft  :");
	for (int i=0; i<n; ++i)
		printf(" %+6.2f%+6.2f",
		  crealf(output_fft[i]), cimagf(output_fft[i]));
	printf("\n");
}
