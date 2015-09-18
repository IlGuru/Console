#ifndef _SCREEN

#define _SCREEN

//#define THREAD_SCREEN_REPAINT

#include <stdio.h>

#ifdef THREAD_SCREEN_REPAINT	
#include <pthread.h>
#endif

#include "./display.h"
#include "./common.h"
#include "./bit_oper.h"

#define SCR_XMAX_SIZE ( 40 )
#define SCR_YMAX_SIZE ( 15 )
#define SCR_POSX_MIN  0
#define SCR_POSY_MIN  0
#define SCR_POSX_MAX  ( SCR_XMAX_SIZE - 1 )
#define SCR_POSY_MAX  ( SCR_YMAX_SIZE - 1 )

#define SCR_MS_REFRESH	100

#define f_srcFirstColumn	0
#define f_srcFirstRow		1
#define f_srcLastColumn		2
#define f_srcLastRow		3
#define f_srcRepainting  	6
#define f_srcContentChanged	7

typedef struct {
	//	Buffer
	unsigned char 	Buffer[SCR_XMAX_SIZE*SCR_YMAX_SIZE];

	//	Registri

	curscoord	x_pos;
	curscoord	y_pos;
	curscoord	x_max;
	curscoord	y_max;

	char		status;
} t_screen;

t_screen *p_screen;

//----------------------------------------------------

#ifdef THREAD_SCREEN_REPAINT

pthread_t 	thScreenRepaint;

void *th_DoScreenRepaint( void *param );

void scrCreateThread( FNINPUT fCallBack );

#endif

void scrInit( FNINPUT fScreenRepaint );

//----------------------------------------------------

void scrIncX();

void scrIncY();

void scrWrite( unsigned char c );

//----------------------------------------------------

void scrRepaint();

#endif	//	_SCREEN