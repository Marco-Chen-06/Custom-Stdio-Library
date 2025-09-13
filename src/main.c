#include "mylib.h"

int main() {
	if (myfopen("testfile.txt", "r") == NULL) {
		perror("Error: ");
		return -1;
	}
	printf("successs");
	return 0;
}
