#ifndef _SCREEN

#define _SCREEN

#include <stdio.h>
#include "./display.h"
#include "./common.h"

// #include "./keyboard.h"

#define SCR_XMAX_SIZE ( 16 )
#define SCR_YMAX_SIZE ( 2 )
#define SCR_POSX_MIN  0
#define SCR_POSY_MIN  0
#define SCR_POSX_MAX  ( SCR_XMAX_SIZE - 1 )
#define SCR_POSY_MAX  ( SCR_YMAX_SIZE - 1 )

#define f_srcFirstColumn	0
#define f_srcFirstRow		1
#define f_srcLastColumn		2
#define f_srcLastRow		3
#define f_srcContentChanged	7

// #define THREAD_SCREEN_REPAINT

typedef struct {
	//	Buffer
	unsigned char 	Buffer[SCR_XMAX_SIZE*SCR_YMAX_SIZE];

	//	Registri
	char		status;

	curscoord	x_pos;
	curscoord	y_pos;
	curscoord	x_max;
	curscoord	y_max;

	curscoord	x;
	curscoord	y;
	curscoord	xs;
	curscoord	xe;
	curscoord	ys;
	curscoord	ye;

} t_screen;

t_screen *p_screen;

//----------------------------------------------------

void scrInit();

//----------------------------------------------------

void scrIncX();

void scrIncY();

void scrWrite( unsigned char c );

//----------------------------------------------------

void scrRepaint();

#endif
