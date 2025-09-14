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
		// for reading, stream starts with 0 bytes left to read
		pStream->remaining = 0;
	}

	//open the file for writing
	if (*mode == 'w') {
		if ((pStream->fd = open(pathname, O_WRONLY)) < 0) {
			return NULL;
		}
		// for writing, stream starts with BUFSIZ bytes left to write
		pStream->remaining = BUFSIZ;
	}
	
	// initialize the buffer data structure
	pStream->base = &(pStream->buf[0]);
	pStream->ptr = &(pStream->buf[0]);

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

	pStream->remaining = 0;

	pStream->mode = *mode; 
	pStream->fd = filedesc;

	
	return pStream;
}

int myfgetc(struct MYSTREAM *stream) {
	// make sure fd of stream was opened for read only
	if ((fcntl(stream->fd, F_GETFL) & O_ACCMODE) != O_RDONLY) {
		errno = EBADF;
		return -1;
	}

	// if no data in buffer, populate it with at most BUFSIZ bytes of data 	
	if (stream->remaining <= 0) {
		// handle failure of read system call
		if ((stream->remaining = read(stream->fd, stream->buf, BUFSIZ)) < 0) {
			return -1;
		}

		// if end of file has been reached, return the value EOF
		if (stream->remaining == 0) {
			return -1;
		}

		// if data was read from the file, initialize pointers
		stream->end = stream->base + stream->remaining;
		stream->ptr = stream->base;
	}

	// update buffer data structure and return the requested character
	int output_char;
	output_char = *(stream->ptr++);
	stream->remaining = stream->end - stream->ptr;
	return output_char;
}

int myfputc(int c, struct MYSTREAM *stream) {
	// make sure fd of stream was opened for write only
	if ((fcntl(stream->fd, F_GETFL) & O_ACCMODE) != O_WRONLY) {
		errno = EBADF;
		return -1;
	}

	if (stream->remaining <= 0) {
		int bytes_written;
		if ((bytes_written = write(stream->fd, stream->buf, BUFSIZ)) <= 0) {
			// handle failure of write system call
			return -1;
		} else if (bytes_written < 4096) {
			// treat partial write as an error
			return -1;
		}
		stream->ptr = stream->base;
	}

	*(stream->ptr++) = c;
	stream->remaining = stream->end - stream->ptr;
	return c;
}

int myfclose(struct MYSTREAM *stream) {
	// if fd of stream was opened for read only, close fd
	if ((fcntl(stream->fd, F_GETFL) & O_ACCMODE) == O_RDONLY) {
		if (close(stream->fd) < 0) {
			return -1;
		}
		free(stream);
		stream = NULL;
		return 0;
	}
	// if fd of stream was opened for write only, flush buffer and close fd
	if ((fcntl(stream->fd, F_GETFL) & O_ACCMODE) == O_WRONLY) {
		int bytes_to_write = stream->ptr - stream->base;
		int bytes_written;
		if ((bytes_written = write(stream->fd, stream->buf, bytes_to_write)) <= 0) {
			// handle failure of write system call
			return -1;
		} else if (bytes_written < bytes_to_write) {
			// treat partial write as an error
			return -1;
		}
		
		if (close(stream->fd) < 0) {
			return -1;
		}
		free (stream);
		stream = NULL;
		return 0;
	}
}
