#ifndef _COMMON

#define _COMMON

#include <time.h>

typedef unsigned char 	uchar;

//typedef unsigned short int curscoord;
typedef uchar 	curscoord;

typedef void (*FNINPUT)(void);

//------------------------------------------------

void sleepMs( long ms );

#endif