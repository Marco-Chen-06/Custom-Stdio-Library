#include "tabstop.h"

// helper function used to write all relevant elements from a buffer to an output stream
int write_to_output(struct MYSTREAM *outputStream, int bytes_filled, char *buf) {
	int index = 0;
	while (index < bytes_filled) {
		myfputc(buf[index++], outputStream);
	}
	return 0;
}

/*
 * Process data from stdin, and write it to outfile or stdout depending on hasOutfile.
 * Only a maximum of BUFSIZ bytes can be processed from stdin.
 * Note: Process is used as a verb and not a noun in this naming convention.
 */
int process_stdin(bool hasOutfile, char *outfile) {
	struct MYSTREAM *inputStream;
	struct MYSTREAM *outputStream;
	char c;

	// buf is BUFSIZ*4 bytes to avoid someone typing BUFSIZ tabs and overflowing buffer
	char buf[BUFSIZ*4];
	int index = 0; // current index of buf (also represents num bytes to write)

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

	// close outputStream, return -1 if failure
	if (myfclose(outputStream) < 0) {
		perror("myfclose error in process_stdin: ");
		return -1;
	}

	return 0;
}

/*
 * Process data from infile, and write it to outfile or stdout depending on hasOutfile.
 * In contract to process_stdin, more than BUFSIZ bytes can be processed from infile.
 * Note: Process is used as a verb and not a noun in this naming convention.
 */
int process_infile(bool hasOutfile, char *outfile, char *infile) {
	struct MYSTREAM *inputStream;
	struct MYSTREAM *outputStream;
	char c;

	// buf is still BUFSIZ*4 bytes to avoid a file with BUFSIZ tabs and overflowing buf
	char buf[BUFSIZ*4];
	int index = 0; // current index of buf (also represents num bytes to write)
	int count = 0; // current number of bytes read from infile

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

	while ((c = myfgetc(inputStream)) != EOF) {
		if (count >= BUFSIZ) {
			// write contents of buf to outputStream
			write_to_output(outputStream, index, buf);
			
			/*
			 * reinitialize counters, and leave buf as is because its data 
			 * will be overwritten by the next write syscalls
			 */
			count = 0;
			index = 0;
		}
		
		if (c == '\t') {
			for (int i = 0; i < 4; i++) {
				buf[index++] = ' ';
			}
		} else {
			buf[index++] = c;
		}
		count++;
	}
	write_to_output(outputStream, index, buf);


	// close outputStream return -1 if failure
	if (myfclose(outputStream) < 0) {
		perror("myfclose error in process_infile: ");
		return -1;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	struct MYSTREAM *stream;
	int opt = 0;

	bool hasOutfile = false;
	char *outfile = NULL; // points to the outfile, if it is provided
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
		// open infile if there is one non-option argument, return 255 if failure
		if (process_infile(hasOutfile, outfile, argv[optind])) {
			return 255;
		}
		return 0;
	}
	return 0;
}
