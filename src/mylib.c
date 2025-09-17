#include "mylib.h"
#include <errno.h>

struct MYSTREAM *myfopen(const char *pathname, const char *mode) {
	// make sure mode is either "r" or "w", return NULL if not
	if ((mode == nullptr) || ((*mode != 'r') && (*mode != 'w'))) {
		errno = EINVAL;
		return NULL;
	}

	// allocate memory for the stream
	struct MYSTREAM *stream;
	stream = (struct MYSTREAM *)malloc(sizeof(struct MYSTREAM));

	// open the file for reading
	if (*mode == 'r') {
		// open file for read only, return NULL if not
		if ((stream->fd = open(pathname, O_RDONLY)) < 0) {
			return NULL;
		}
		// for reading, stream starts with 0 bytes left to read
		stream->remaining = 0;
	}

	//open the file for writing
	if (*mode == 'w') {
		// open file for write only, return NULL if not
		if ((stream->fd = open(pathname, O_WRONLY)) < 0) {
			return NULL;
		}
		// for writing, stream starts with BUFSIZ bytes left to write
		stream->remaining = BUFSIZ;
	}
	
	// initialize the buffer data structure
	stream->base = &(stream->buf[0]);
	stream->ptr = &(stream->buf[0]);

	stream->mode = *mode; 

	return stream;
}

struct MYSTREAM *myfdopen(int filedesc, const char *mode) {
	// make sure mode is either "r" or "w", return NULL if not
	if ((mode == nullptr) || ((*mode != 'r') && (*mode != 'w'))) {
		errno = EINVAL;
		return NULL;
	}

	// make sure the file descriptor is actually open, return NULL if not
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
	// make sure fd of stream was opened for read only, return -1 if not
	if (stream->mode != 'r') {
		errno = EBADF;
		return -1;
	}

	// if no data in buffer, populate it with at most BUFSIZ bytes of data 	
	if (stream->remaining <= 0) {
		// read BUFSIZ bytes of data, return -1 if failure
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
	// make sure fd of stream was opened for write only, return -1 if not
	if (stream->mode != 'w') {
		errno = EBADF;
		return -1;
	}

	if (stream->remaining <= 0) {
		int bytes_written;
		// write BUFSIZE bytes of data to the stream, return -1 if failure
		if ((bytes_written = write(stream->fd, stream->buf, BUFSIZ)) <= 0) {
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
	if (stream->mode == 'r') {
		// close fd of stream, return -1 if failure
		if (close(stream->fd) < 0) {
			return -1;
		}
		free(stream);
		stream = NULL;
		return 0;
	}
	// if fd of stream was opened for write only, flush buffer and close fd
	if (stream->mode == 'w') {
		int bytes_to_write = stream->ptr - stream->base;
		int bytes_written;
		// write remaining bytes to fd, return -1 if failure
		if ((bytes_written = write(stream->fd, stream->buf, bytes_to_write)) <= 0){
			return -1;
		} else if (bytes_written < bytes_to_write) {
			// treat partial write as an error
			return -1;
		}
		
		// close fd of stream, return -1 if failure
		if (close(stream->fd) < 0) {
			return -1;
		}
		free(stream);
		stream = NULL;
		return 0;
	}
	/*
	 * The program should only reach this condition if the file was
	 * not opened for either read or write only. Nonetheless, this 
	 * indicates an error.
	 */
	return -1;
}
