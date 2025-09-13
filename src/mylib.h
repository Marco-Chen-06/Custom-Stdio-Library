#ifndef MY_LIB_H
#define MY_LIB_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>

// based on GNU libc IO_FILE structure
struct MYSTREAM{
	char *ptr; // curent pointer to buffer
	char *base; // base address of buffer
	char *end; // end address of buffer
	char buf[4096]; // stores read and write data
	int remaining;  // number of characters left in the buffer
	char flag; // read, write, or error
	int fd; // file descriptor
};

struct MYSTREAM *myfopen(const char *pathname, const char *mode);

#endif
