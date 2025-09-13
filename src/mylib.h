#ifndef MY_LIB_H
#define MY_LIB_H

#include <stdio.h>

typedef struct {
} MYSTREAM;

struct MYSTREAM *myfopen(const char *pathname, const char *mode);

#endif
