#ifndef MY_LIB_H
#define MY_LIB_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// based on GNU libc IO_FILE structure
struct MYSTREAM{
	char *ptr; // curent pointer to buffer
	char *base; // base address of buffer
	char *end; // end address of buffer
	int remaining;  // number of characters left in the buffer
	int flags; // read, write, or error
};

struct MYSTREAM *myfopen(const char *pathname, const char *mode);

#endif
