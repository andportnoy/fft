int *argsort(float *data, int n) {
	int *index = malloc(n * sizeof*index);
	for (int i=0; i<n; ++i)
		index[i] = i;
	int cmp(const void *a, const void *b) {
		int ai = *(int *)a;
		int bi = *(int *)b;
		return data[ai] >data[bi]? -1:
		       data[ai]==data[bi]?  0: 1;
	}
	qsort(index, n, sizeof*index, cmp);
	return index;
}

/* In the naive implementation, we can process all chunks separately and
 * average the magnitudes, then perform top-k selection.
 * - Read the data
 * - Allocate fourier matrix of size NFRAMES.
 * - Run DFT on all chunks independently
 * - Average DFT magnitudes
 * - Find top k frequencies in the average
 * - Extract only those
 * - Run IDFT on those frequencies
 * - Write the data
 */
void compress(const patype *input, patype *output, int nchunk, int k) {
	float complex *fm  =         fourier_matrix(NFRAMES);
	float complex *ifm = inverse_fourier_matrix(NFRAMES);


	float (*inmat)[NFRAMES] = (float (*)[])input;

	float complex (*dftoutput)[NFRAMES]  = malloc(nchunk*sizeof*dftoutput);
	float complex (*intermediate)[NFRAMES] = calloc(nchunk, sizeof*intermediate);
	float (*idftoutput)[NFRAMES] = malloc(nchunk*sizeof*idftoutput);

	float *mag    = malloc(NFRAMES * sizeof*mag);
	float *magsum = calloc(NFRAMES, sizeof*magsum);

	for (int i=0; i<nchunk; ++i) {
		dft(fm, (float *)&inmat[i], (float complex *)&dftoutput[i], NFRAMES, NFRAMES);
		magnitude((float complex *)&dftoutput[i], mag, NFRAMES);
		for (int j=0; j<NFRAMES; ++j)
			magsum[j] += mag[j];
	}

	int *top = argsort(magsum, NFRAMES);

	for (int i=0; i<NFRAMES; ++i)
		printf("orig: (%5d, %6.2f) sorted: (%5d, %6.2f)\n", i, magsum[i], top[i], magsum[top[i]]);

	for (int i=0; i<nchunk; ++i)
	for (int j=0; j<k; ++j)
		intermediate[i][top[j]] = dftoutput[i][top[j]];

	for (int i=0; i<nchunk; ++i)
		idft(ifm, (float complex *)&intermediate[i], (float *)&idftoutput[i], NFRAMES, NFRAMES);

	for (int i=0, n=nchunk*NFRAMES; i<n; ++i)
		output[i] = idftoutput[i/NFRAMES][i%NFRAMES];
}

/* 1. Read the data.
 * 2. Run DFT.
 * 3. Extract k top frequencies by magnitude.
 * 4. Run inverse DFT.
 * 5. Write the data.
 */
int main(int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "usage: %s <infile> <outfile> <k>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char *infilename = argv[1];
	char *outfilename = argv[2];
	int k = atoi(argv[3]);

	patype *input;
	int nchunk = chunkread(infilename, &input);

	patype *output = malloc(nchunk*NFRAMES*sizeof*output);
	compress(input, output, nchunk, k);

	chunkwrite(outfilename, output, nchunk);
}
