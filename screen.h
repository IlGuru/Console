#include <stdio.h>

#ifndef _SCREEN

#define _SCREEN

#define scr_xmax 15
#define scr_ymax 2

typedef struct {
	//	Buffer
	unsigned char 	Buffer[scr_xmax*scr_ymax];
	//	Registri
	short int	x_pos;
	short int	y_pos;
	short int	x_max;
	short int	y_max;
	short int	BufSize;
} t_scrMem;

t_scrMem *scrMem;

//----------------------------------------------------
void scr_Repaint(int amount);

#endif
