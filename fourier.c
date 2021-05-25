#define FACTN 64

static float factorial[FACTN] = {1};
void factorial_initialize(void) {
	for (int i=1, n=FACTN; i<n; ++i)
		factorial[i] = factorial[i-1]*i;
}

float complex cexpo(float complex x) {
	float complex res = 0, num = 1;
	for (int i=0, n=FACTN; i<n; ++i) {
		res += num/factorial[i];
		num *= x;
	}
	return res;
}

void cprint(float complex x) {
	printf("%f + %fi", crealf(x), cimagf(x));
}

void cmprint(float complex *m, int n) {
	float complex (*mat)[n] = (float complex (*)[n])m;
	for (int i=0; i<n; ++i) {
		for (int j=0; j<n; ++j) {
			if (j)
				printf(" ");
			printf("%+.2f%+.fi", crealf(mat[i][j]), cimagf(mat[i][j]));
		}
		printf("\n");
	}
}

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

int dread(FILE *f, float **x) {
	fseek(f, 0, SEEK_END);
	int b = ftell(f);
	fseek(f, 0, SEEK_SET);

	assert(b%sizeof (float) == 0);
	int n = b / sizeof (float);

	float *xx = malloc(n * sizeof *xx);
	fread(xx, n*sizeof*xx, 1, f);
	*x = xx;

	return n;
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
			printf(mag[i]>5? "#": " ");
		printf("\n");
	}
	audio_terminate();
}

int main2(int argc, char **argv) {
	factorial_initialize();

	assert(argc == 3);

	char *in = argv[1];
	FILE *fi = fopen(in, "r");
#if 0
	float *x;
	int n = dread(fi, &x);
#else
	fclose(fi);
	int n = 2048;
	float *x = malloc(n * sizeof *x);
#endif

	float complex *y = malloc(n * sizeof *y);
	float complex *fm = fourier_matrix(n);

	double max = 0.;
	for (;;) {
		dft(fm, x, y, n);
		double ms = msdiff();
		max = max<ms? ms: max;
		printf("%f %f ms\n", ms, max);
	}

	char *out = argv[2];
	FILE *fo = fopen(out, "w");
	fwrite(y, n*sizeof*y, 1, fo);
	fclose(fo);
}
