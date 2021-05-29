/* Read file, populate buffer pointer, return how many chunks of size NFRAMES
 * were read.
 */
int chunkread(char *filename, patype **buffer) {
	FILE *f = fopen(filename, "r");
	fseek(f, 0, SEEK_END);
	long nbytes = ftell(f);
	fseek(f, 0, SEEK_SET);
	patype *ptr = malloc(nbytes);
	fread(ptr, nbytes, 1, f);
	fclose(f);

	*buffer = ptr;
	return nbytes/sizeof(patype)/NFRAMES;
}

void chunkwrite(char *filename, patype *buffer, int nchunk) {
	FILE *f = fopen(filename, "w");
	fwrite(buffer, nchunk*NFRAMES*sizeof*buffer, 1, f);
	fclose(f);
}
