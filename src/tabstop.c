#include "tabstop.h"

// helper function used to write all relevant elements from a buffer to an output stream
int write_to_output(struct MYSTREAM *outputStream, int bytes_filled, char *buf) {
	int index = 0;
	while (index <= bytes_filled) {
		myfputc(buf[index++], outputStream);
	}
	return 0;
}

// process data from standard input, 
int process_stdin(bool hasOutfile, char *outfile) {
	struct MYSTREAM *inputStream;
	struct MYSTREAM *outputStream;
	char c;

	// buf is BUFSIZ*4 bytes to avoid someone typing BUFSIZ tabs and overflowing buffer
	char buf[BUFSIZ*4];
	char index = 0; 

	// open stdin, return -1 if failure
	if ((inputStream = myfdopen(0, "r")) == NULL) {
		perror("myfdopen error in process_stdin: ");
		return -1;
	}

	if (hasOutfile) {
		// open outfile, return -1 if failure
		if ((outputStream = myfopen(outfile, "w")) == NULL) {
			perror("myfopen error in process_stdin: ");
			return -1;
		}
	} else {
		// open stdout, return -1 if failure
		if ((outputStream = myfdopen(1, "w")) == NULL) {
			perror("myfdopen error in process_stdin: ");
			return -1;
		}
	}


	// process standard input and put it into buffer
	while ((c = myfgetc(inputStream)) != EOF) {
		if (c == '\t') {
			for (int i = 0; i < 4; i++) {
				buf[index++] = ' ';
			}
		} else {
			buf[index++] = c;
		}
	}	

	/*
	 * write contents of buf to outputStream. Index acts as the number of
	 * bytes_filled in this function call.
	 */
	write_to_output(outputStream, index, buf);

	// close outfile if opened, return -1 if unsuccessful
	if (hasOutfile) {
		if (myfclose(outputStream) < 0) {
			perror("myfclose error in process_stdin: ");
			return -1;
		}
	}
	return 0;
}

int process_infile(bool hasOutfile, char *outfile, char *infile) {
	struct MYSTREAM *inputStream;
	struct MYSTREAM *outputStream;
	char c;

	// buf is still BUFSIZ*4 bytes to avoid a file with BUFSIZ tabs and overflowing buf
	char buf[BUFSIZ*4];
	char index = 0; // current index of buf
	char count = 0; // current number of bytes read from infile

	// open infile, return -1 if failure
	if ((inputStream = myfopen(infile, "r")) == NULL) {
		perror("myfopen error in process_infile: ");
		return -1;
	}

	if (hasOutfile) {
		// open outfile, return -1 if failure
		if ((outputStream = myfopen(outfile, "w")) == NULL) {
			perror("myfopen error in process_infile");
			return -1;
		}
	} else {
		// open stdout, return -1 if failure
		if ((outputStream = myfdopen(1, "w")) == NULL) {
			perror("myfdopen error in process_infile: ");
			return -1;
		}
	}

	//while ((c = myfgetc(stream)) != EOF) {
	//	// make sure buf doesn't overflow and more bytes can be processed
	//	if (count >= BUFSIZ) {
//
//		}
//
//	}
	return 0;
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

	// there shouldn't be more than 1 non-option argument. If there is, return 255.
	if ((argc - optind) > 1) {
		return 255;
	}

	// if there is no non-option argument, open stdin for writing. Otherwise, open infile.
	if ((argc - optind) == 0) {
		// open standard input, return 255 if failure
		if (process_stdin(hasOutfile, outfile)) {
			return 255;
		}
	} else if ((argc - optind) == 1) {
		// open infile, return 255 if failure
		if (process_infile(hasOutfile, outfile, argv[optind])) {
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
