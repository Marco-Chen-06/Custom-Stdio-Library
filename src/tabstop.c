#include "tabstop.h"

// process data from standard input, 
int process_stdin(bool hasOutfile, char *outfile) {
	struct MYSTREAM *stream;
	char c;

	// buf is BUFSIZ*4 bytes to avoid someone typing BUFSIZ tabs and overflowing buffer
	char buf[BUFSIZ*4];
	char index; 

	// open file, return -1 if unsuccessful
	if ((stream = myfdopen(0, "r")) == NULL) {
		perror("myfdopen error in process_stdin: ");
		return -1;
	}

	// process standard input and put it into buffer
	while ((c = myfgetc(stream)) != EOF) {
		if (c == '\t') {
			for (int i = 0; i < 4; i++) {
				buf[index++] = ' ';
			}
		} else {
			buf[index++] = c;
		}
	}	

	if (hasOutfile) {
		// write to outfile, return -1 if failure
		if ((stream = myfopen(outfile, "w")) == NULL) {
			perror("myfopen error in process_stdin: ");
			return -1;
		}
	} else {
		// write to stdout, return -1 if failure
		if ((stream = myfdopen(1, "w")) == NULL) {
			perror("myfdopen error in process_stdin: ");
			return -1;
		}
	}

	/*
	 * Current index is the number of bytes filled in buf, so 
	 * we can treat index like a constant representing the bytes filled,
	 * and use reverse_index to index the buffer and write sequentially to
	 * either stdout or outfile.
	 */
	int reverse_index;
	while (reverse_index <= index) {
		myfputc(buf[reverse_index++], stream);
	}

	// close outfile if opened, return -1 if unsuccessful
	if (hasOutfile) {
		if (myfclose(stream) < 0) {
			perror("myfclose error in process_stdin: ");
			return -1;
		}
	}
	return 0;
}

int process_infile(bool hasOutfile, char*outfile) {
	struct MYSTREAM *stream;
	char c;

	// buf is still BUFSIZ*4 bytes to avoid someone typing BUFSIZ tabs and overflowing
	char buf[BUFSIZ*4];
	char index;
	
}

int main(int argc, char* argv[]) {
	struct MYSTREAM *stream;
	int opt;

	bool hasOutfile;
	char *outfile; // points to the outfile, if it is provided
	while ((opt = getopt(argc, argv, ":o:")) != -1) {
		switch(opt) {
			case 'o':
				hasOutfile = true;
				outfile = optarg;
				break;
			case ':':
				// return error if no option after -o detected
				return 255;
			case '?':
				// return error if flag is anything other than -o
				return 255;
		}
	}

	// make sure there isn't more than one non-option argument
	if ((argc - optind) > 1) {
		return -1;
	}

	// if there is no non-option argument, open stdin for writing. Otherwise, open infile.
	if ((argc - optind) == 0) {
		// open standard input, return 255 if failure
		if (process_stdin(hasOutfile, outfile)) {
			return 255;
		}
	} else if ((argc - optind) == 1) {
		// open infile, return 255 if failure
		if (process_infile(hasOutfile, outfile)) {
			return 255;
		}
		return 0;
	}

	for (int i = optind; i < argc; i++) {
		printf("argcount: %d\n", argc);
		printf("optind: %d\n", optind);
		// printf("extra argument: %s\n", argv[i]);
	}
	return 0;
}
