#include "mylib.h"

int test_myfgetc();

int main() {
	if (test_myfgetc() < 0) {
		return -1; 
	}

	return 0;
}

int test_myfgetc() {
	struct MYSTREAM *fStream;
	int received_char;
	if ((fStream = myfopen("testfile.txt", "r")) == NULL) {
		perror("open Error: ");
		return -1;
	}
	while ((received_char = myfgetc(fStream)) != -1) {
		printf("%c", received_char);
	}
	printf("\nreached end of file, success\n");
	return 0;
}
