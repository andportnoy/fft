#define FACTN 64

static double factorial[FACTN] = {1};
void factorial_initialize(void) {
	for (int i=1, n=FACTN; i<n; ++i)
		factorial[i] = factorial[i-1]*i;
}

double complex cexpo(double complex x) {
	double complex res = 0, num = 1;
	for (int i=0, n=FACTN; i<n; ++i) {
		res += num/factorial[i];
		num *= x;
	}
	return res;
}

void cprint(double complex x) {
	printf("%f + %fi", creal(x), cimag(x));
}

void cmprint(double complex *m, int n) {
	double complex (*mat)[n] = (double complex (*)[n])m;
	for (int i=0; i<n; ++i) {
		for (int j=0; j<n; ++j) {
			if (j)
				printf(" ");
			printf("%+.2f%+.fi", creal(mat[i][j]), cimag(mat[i][j]));
		}
		printf("\n");
	}
}

double complex *fourier_matrix(int n) {
	double complex (*fm)[n] = malloc(n*sizeof *fm);
	for (int k=0; k<n; ++k) /* every row corresponds to a given k */
	for (int j=0; j<n; ++j) {
		fm[k][j] = cexpo(-2*M_PI*I*k*j/n);
	}
	return (double complex *)fm;
}

void matvec(const double complex * restrict m, const double * restrict x, double complex * restrict y, int n) {
	double complex (*mat)[n] = (double complex (*)[])m;
	for (int i=0; i<n; ++i) {
		y[i] = 0;
		for (int j=0; j<n; ++j) {
			y[i] += mat[i][j] * x[j];
		}
	}
}

void dft(const double complex * restrict fm, const double * restrict x, double complex * restrict y, int n) {
	matvec(fm, x, y, n);
}

int dread(FILE *f, double **x) {
	fseek(f, 0, SEEK_END);
	int b = ftell(f);
	fseek(f, 0, SEEK_SET);

	assert(b%sizeof (double) == 0);
	int n = b / sizeof (double);

	double *xx = malloc(n * sizeof *xx);
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

int main(int argc, char **argv) {
	factorial_initialize();

	assert(argc == 3);

	char *in = argv[1];
	FILE *fi = fopen(in, "r");
#if 0
	double *x;
	int n = dread(fi, &x);
#else
	fclose(fi);
	int n = 2048;
	double *x = malloc(n * sizeof *x);
#endif

	double complex *y = malloc(n * sizeof *y);
	double complex *fm = fourier_matrix(n);

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
