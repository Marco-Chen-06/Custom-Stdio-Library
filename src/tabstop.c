#include "tabstop.h"

// simply opens standard input, and reads, then closes the file
int process_stdin() {
	// check if file opening was unsuccessful
	if (myfdopen(0, "w") == NULL) {
		return -1;
	}



}

int main(int argc, char* argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "o:")) != -1) {
		switch(opt) {
			case 'o':
				printf("found output with %s\n", optarg);
				break;
			case '?':
				return 255;
		}
	}

	// make sure there isn't more than one non-option argument
	if ((argc - optind) > 1) {
		return -1;
	}

	if ((argc - optind) == 1) {
		process_stdin();
	}

	for (int i = optind; i < argc; i++) {
		printf("argcount: %d\n", argc);
		printf("optind: %d\n", optind);
		// printf("extra argument: %s\n", argv[i]);
	}
	return 0;
}
