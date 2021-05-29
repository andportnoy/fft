int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <seconds> <outfile>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int nsec = atoi(argv[1]);
	printf("nsec=%d\n", nsec);
	char *filename = argv[2];

	int nchunk = (nsec * SRATE)/NFRAMES + 1;
	int totalframes = NFRAMES * nchunk;
	patype buffer[totalframes];
	audio_initialize();
	for (int framei=0; framei<totalframes; framei+=NFRAMES) {
		patype *chunk = audio_record();
		memcpy(&buffer[framei], chunk, NFRAMES*sizeof*chunk);
	}
	chunkwrite(filename, buffer, nchunk);
	audio_terminate();
}
