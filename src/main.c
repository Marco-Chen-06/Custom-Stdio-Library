#include "mylib.h"

int main() {
	if (myfopen("file.txt", "r") == NULL) {
		printf("disaster");
	}
	return 0;
}
