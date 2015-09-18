#include <stdio.h>
#include <stdlib.h>

#ifndef _DISPLAY

#define _DISPLAY

#define DSP_XMAX_SIZE 64
#define DSP_YMAX_SIZE 16

typedef struct {
	//	Buffer
	unsigned char 	Buffer[DSP_XMAX_SIZE*DSP_YMAX_SIZE];
	//	Registri
	short int	x_pos;
	short int	y_pos;
	short int	x_max;
	short int	y_max;
} t_dspMem;

t_dspMem *dspMem;

//----------------------------------------------------

void dsp_init();

//----------------------------------------------------

void dsp_inc_xpos();
void dsp_inc_ypos();

short int dsp_xmax();
short int dsp_ymax();

short int dsp_xget();
void dsp_xset( short int p );

short int dsp_yget();
void dsp_yset( short int p );

unsigned char dsp_read();
void dsp_write( unsigned char c );

void dsp_repaint();

#endif
