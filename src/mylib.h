#ifndef MY_LIB_H
#define MY_LIB_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// somewhat based on GNU libc IO_FILE structure
struct MYSTREAM{
	char* ptr; // pointer to current index of buffer
	char* base; // base address of buffer
	char* end; // address of last element of buffer
	char buf[BUFSIZ]; // stores read or write data
	int remaining;  // number of characters left in the buffer
	char mode; // read, write, or error
	int fd; // file descriptor
};

struct MYSTREAM *myfopen(const char *pathname, const char *mode);
struct MYSTREAM *myfdopen(int filedesc, const char *mode);
int myfgetc(struct MYSTREAM *stream);
int myfputc(int c, struct MYSTREAM *stream);

#endif
