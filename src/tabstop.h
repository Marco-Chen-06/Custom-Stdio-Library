/*
 * tabstop.h -- process input one character at a time, replace tabs with 4 spaces
 * Cooper Union ECE357 Operating Systems Pset1
 */

#include "mylib.h"
int write_to_output(struct MYSTREAM *outputStream, int bytes_filled, char *buf);
int process_stdin(bool hasOutfile, char *outfile);
int process_infile(bool hasOutfile, char *outfile, char *infile);

