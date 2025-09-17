/*
 * This main file is only for testing basic mylib.c functionality. It is not rigorous
 * at all and really doesn't do much. 
 * tabstop.c is the actual file that makes use of the mylib.c
 */

#include "mylib.h"

int test_myfgetc();
int test_myfputc();

int main() {
	// if (test_myfgetc() < 0) {
	// 	return -1; 
	// }
	if (test_myfputc() < 0) {
		return -1;
	}
	return 0;
}

int test_myfgetc() {
	struct MYSTREAM *stream;
	int received_char;
	if ((stream = myfopen("testfile.txt", "r")) == NULL) {
		perror("open error: ");
		return -1;
	}
	while ((received_char = myfgetc(stream)) != -1) {
		printf("%c", received_char);
	}
	printf("\nreached end of file, success\n");
	return 0;
}

// write 5000 characters to a file then close it
int test_myfputc() {
	struct MYSTREAM *stream;
	if ((stream = myfopen("writetest.txt", "w")) == NULL) {
		perror("write error: ");
		return -1;
	}
	int count;
	while (count < 5000) {
		if (count < 4096) {
			myfputc('L', stream);
		} else if (count < 5000) {
			myfputc('G', stream);
		} else {
			perror("somehing terrible happened");
			return -1;
		}
		count += 1;
	}
	if (myfclose(stream) < 0) {
		perror("myfclose error: ");
		return -1;
	}

	printf("\n wrote 5000 characters, success\n");
	return 0;
}

