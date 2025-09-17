#include "tabstop.h"

// process data from standard input, 
int process_stdin(bool hasOutfile, char *outfile) {
	struct MYSTREAM *stream;

	// open file, return -1 if unsuccessful
	if ((stream = myfopen(0, "w")) == NULL) {
		perror("error: ");
		return -1;
	}


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
				printf("found output with %s\n", outfile);
				break;
			case ':':
				printf("NO OPTION FOR AN O DETECTED!!");
				return 255;
			case '?':
				return 255;
		}
	}

	// make sure there isn't more than one non-option argument
	if ((argc - optind) > 1) {
		return -1;
	}

	// if there is no non-option argument, open stdin for writing
	if ((argc - optind) == 0) {
		// open standard input, return NULL if failure
		if (process_stdin(hasOutfile, outfile)) {
			return 255;
		}
	} else if ((argc - optind) == 1) {
		// if there is one non-option argument, open infile for writing
		return 0;
	}

	for (int i = optind; i < argc; i++) {
		printf("argcount: %d\n", argc);
		printf("optind: %d\n", optind);
		// printf("extra argument: %s\n", argv[i]);
	}
	return 0;
}
