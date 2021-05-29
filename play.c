int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <infile>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char *filename = argv[1];

	patype *buffer;
	int niter = chunkread(filename, &buffer);

	audio_initialize();
	for (int i=0; i<niter; ++i) {
		audio_play(&buffer[i*NFRAMES]);
	}
	audio_terminate();
}
