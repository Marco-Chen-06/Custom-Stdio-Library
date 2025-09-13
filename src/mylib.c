#include "mylib.h"
#include <errno.h>

struct MYSTREAM *myfopen(const char *pathname, const char *mode) {
	// make sure mode is either "r" or "w", and nothing else
	if ((mode == nullptr) || ((*mode != 'r') && (*mode != 'w'))) {
		errno = EINVAL;
		return NULL;
	}
	struct MYSTREAM *pStream;
	pStream = (struct MYSTREAM *)malloc(sizeof(struct MYSTREAM));
	
	return pStream;
}
