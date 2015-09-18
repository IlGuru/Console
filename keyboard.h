#ifndef _KEYBOARD

#define _KEYBOARD

//#define THREAD_KEYBOARD

#include <stdio.h>

#ifdef __MINGW32__
#include <ncursest/ncurses.h>
#else
#include <curses.h>
#endif

#ifdef THREAD_KEYBOARD	
#include <pthread.h>
#endif

#include "./common.h"
#include "./bit_oper.h"

#define KB_BUFFER_SIZE		8
#define KB_BUFFER_MIN_POS	0
#define KB_BUFFER_MAX_POS	( KB_BUFFER_SIZE - 1 )

#define KB_MS_REFRESH		0

#define KB_BUFFER_EMPTY		0
#define KB_BUFFER_FULL		1
#define KB_PUTCH			2
#define KB_GETCH			3

typedef struct {
	uchar	Buffer[ KB_BUFFER_SIZE ];
	char    BufPos;
	char	status;
} t_keyboard;

t_keyboard *p_keyboard;

void kbPutCh();
uchar kbGetCh();

#ifdef THREAD_KEYBOARD	

pthread_t 	thReadKB;

void *th_DoReadKB( void *param );

void kbCreateThread( FNINPUT fCallBack );

#endif

void kbInit();

#endif
