/* Read file, populate buffer pointer, return how many chunks of size NFRAMES
 * were read.
 */
int chunkread(char *filename, patype **buffer);
void chunkwrite(char *filename, patype *buffer, int nchunk);
