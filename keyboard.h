#ifndef _KEYBOARD

#define _KEYBOARD

// #define THREAD_KEYBOARD

#include <stdio.h>
#include <curses.h>

// #ifdef THREAD_KEYBOARD	
// #include <pthread.h>
// #endif

#include "./common.h"
#include "./bit_oper.h"

#define KB_BUFFER_SIZE		8
#define KB_BUFFER_MIN_POS	0
#define KB_BUFFER_MAX_POS	( KB_BUFFER_SIZE - 1 )

#define KB_BUFFER_EMPTY		0
#define KB_BUFFER_FULL		1
#define KB_PUTCH			2
#define KB_GETCH			3

typedef struct {
	uchar	Buffer[ KB_BUFFER_SIZE ];
	char    BufPos;
	char	status;
// #ifdef THREAD_KEYBOARD	
	// pthread_t 	thReadKB;
// #endif
} t_keyboard;

t_keyboard *p_keyboard;

void kbInit();

#endif
