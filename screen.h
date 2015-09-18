#include <stdio.h>
#include "./display.h"

#ifndef _SCREEN

#define _SCREEN

#define SCR_XMAX_SIZE 20
#define SCR_YMAX_SIZE 4
#define SCR_POSX_MIN  0
#define SCR_POSY_MIN  0
#define SCR_POSX_MAX  ( SCR_XMAX_SIZE - 1 )
#define SCR_POSY_MAX  ( SCR_YMAX_SIZE - 1 )


typedef struct {
	//	Buffer
	unsigned char 	Buffer[SCR_XMAX_SIZE*SCR_YMAX_SIZE];
	//	Registri
	short int	x_pos;
	short int	y_pos;
	short int	x_max;
	short int	y_max;
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
