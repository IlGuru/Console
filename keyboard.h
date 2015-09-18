#ifndef _KEYBOARD

#define _KEYBOARD

#include <stdio.h>
#include <curses.h>
#include "./common.h"
#include "./bit_oper.h"

#define KB_BUFFER_SIZE		8
#define KB_BUFFER_MIN_POS	0
#define KB_BUFFER_MAX_POS	( KB_BUFFER_SIZE - 1 )

#define KB_BUFFER_EMPTY		0
#define KB_BUFFER_FULL		1

typedef struct {
	uchar	Buffer[ KB_BUFFER_SIZE ];
	char    BufPos;
	char	status;
} t_keyboard;

t_keyboard *p_keyboard;

void kbInit();

#endif
