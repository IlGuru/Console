#include <stdio.h>
#include "./display.h"

#ifndef _SCREEN

#define _SCREEN

#define SCR_XMAX_SIZE 64
#define SCR_YMAX_SIZE 16

typedef struct {
	//	Buffer
	unsigned char 	Buffer[SCR_XMAX_SIZE*SCR_YMAX_SIZE];
	//	Registri
	short int	x_pos;
	short int	y_pos;
	short int	x_max;
	short int	y_max;
	// short int	BufSize;
} t_scrMem;

t_scrMem *scrMem;

//----------------------------------------------------

void scrInit();

//----------------------------------------------------

void scrInc_xpos();

void scrInc_ypos();

void scrWrite( unsigned char c );

//----------------------------------------------------

void scrRepaint();

#endif
