#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <pthread.h>

#include "./registry.h"

#ifndef _KEYBOARD

#define _KEYBOARD

#define KeyBoard_BufSize 	8
#define BufferEmpty_fSet 	|| 10000000
#define BufferEmpty_fClr 	&& 01111111
#define BufferEmpty_fChk 	&& 10000000
#define BufferOverflow_fSet || 00000010
#define BufferOverflow_fClr && 11111101
#define BufferOverflow_fChk && 00000010

typedef struct {
	char 			Buffer[KeyBoard_BufSize];
	short int 		BufPos;
	unsigned char 	Status;
} t_kbdMem;

t_kbdMem *kbdMem;

void KeyBoard_Init();

void *KeyBoard_Scan( void *threadid );

#endif