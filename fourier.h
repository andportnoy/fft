float complex *fourier_matrix_custom(
	const float *freq, /* array specifying desired frequencies in Hz */
	int m,             /* length of freq, number of rows in output matrix */
	int n,             /* number of samples in input signal, n columns */
	int srate          /* input signal sample rate in Hz */
);

float complex *fourier_matrix(int n);
float complex *inverse_fourier_matrix(int n);
void dft(
	const float complex * restrict fm,
	const float * restrict x,
	float complex * restrict y,
	int m,
	int n
);

void idft(
	const float complex * restrict ifm,
	const float complex * restrict x,
	float * restrict y,
	int m,
	int n
);

void magnitude(const float complex * restrict x, float * restrict y, int n);
