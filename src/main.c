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
	struct MYSTREAM *fStream;
	int received_char;
	if ((fStream = myfopen("testfile.txt", "r")) == NULL) {
		perror("open error: ");
		return -1;
	}
	while ((received_char = myfgetc(fStream)) != -1) {
		printf("%c", received_char);
	}
	printf("\nreached end of file, success\n");
	return 0;
}

// write 5000 characters to a file then close it
int test_myfputc() {
	struct MYSTREAM *fStream;
	if ((fStream = myfopen("writetest.txt", "w")) == NULL) {
		perror("write error: ");
		return -1;
	}
	int count;
	while (count < 5000) {
		if (count < 4096) {
			myfputc('L', fStream);
		} else if (count < 5000) {
			myfputc('G', fStream);
		} else {
			perror("somehing terrible happened");
			return -1;
		}
		count += 1;
	}
	if (myfclose(fStream) < 0) {
		perror("myfclose error: ");
		return -1;
	}

	printf("\n wrote 5000 characters, success\n");
	return 0;
}

