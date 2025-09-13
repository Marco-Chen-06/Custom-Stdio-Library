#include "mylib.h"

int main() {
// 	// test fopen
// 	 if (myfopen("testfile.txt", "r") == NULL) {
// 		perror("Error: ");
// 		return -1;
// 	}
// 	printf("successs");
// 	return 0;

	int fd;
	if ((fd = myfopen("testfile.txt", "r") == NULL)) {
		perror("open Error: ");
		return -1;
	}
	if (myfdopen(fd, "r") == NULL) {
		perror("myfdopen Error: ");
		return -1;
	}
	printf("success");
}
