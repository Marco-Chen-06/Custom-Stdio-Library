#include "mylib.h"
#include <errno.h>

struct MYSTREAM *myfopen(const char *pathname, const char *mode) {
	// make sure mode is either "r" or "w", and absolutely nothing else
	if ((mode == nullptr) || ((*mode != 'r') && (*mode != 'w'))) {
		errno = EINVAL;
		return NULL;
	}

	// allocate memory for the stream
	struct MYSTREAM *pStream;
	pStream = (struct MYSTREAM *)malloc(sizeof(struct MYSTREAM));

	// open the file for reading
	if (*mode == 'r') {
		if ((pStream->fd = open(pathname, O_RDONLY)) < 0) {
			return NULL;
		}
	}

	//open the file for writing
	if (*mode == 'w') {
		if ((pStream->fd = open(pathname, O_WRONLY)) < 0) {
			return NULL;
		}
	}
	
	// initialize the buffer data structure
	pStream->base = &(pStream->buf[0]);
	pStream->ptr = &(pStream->buf[0]);
	pStream->end = &(pStream->buf[4096]);

	pStream->remaining = pStream->end - pStream->ptr;

	pStream->mode = *mode; 

	return pStream;
}

struct MYSTREAM *myfdopen(int filedesc, const char *mode) {
	// make sure mode is either "r" or "w", and absolutely nothing else
	if ((mode == nullptr) || ((*mode != 'r') && (*mode != 'w'))) {
		errno = EINVAL;
		return NULL;
	}

	// make sure the file descriptor is actually open
	if (fcntl(filedesc, F_GETFL) == -1) {
		errno = EBADF;
		return NULL;
	}

	// allocate memory for the stream
	struct MYSTREAM *pStream;
	pStream = (struct MYSTREAM *)malloc(sizeof(struct MYSTREAM));

	// initialize the buffer data structure
	pStream->base = &(pStream->buf[0]);
	pStream->ptr = &(pStream->buf[0]);
	pStream->end = &(pStream->buf[4096]);

	pStream->remaining = pStream->end - pStream->ptr;

	pStream->mode = *mode; 
	pStream->fd = filedesc;

	
	return pStream;
}
