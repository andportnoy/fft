int main(void) {
	int n = 10;

	float complex (*fm)[n] = (float complex (*)[])fourier_matrix(n); 
	float complex (*ifm)[n] = (float complex (*)[])inverse_fourier_matrix(n);

	float complex (*out)[n] = malloc(n*sizeof *out);

	for (int i=0; i<n; ++i)
	for (int j=0; j<n; ++j) {
		out[i][j] = 0;
		for (int k=0; k<n; ++k) {
			out[i][j] += fm[i][k] * ifm[k][j];
		}
	}

	for (int i=0; i<n; ++i) {
		for (int j=0; j<n; ++j) {
			if (j)
				printf(" ");
			printf("%+6.2f%+6.2f", crealf(out[i][j]), cimagf(out[i][j]));
		}
		printf("\n");
	}
}
