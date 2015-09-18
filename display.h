#ifndef _DISPLAY

#define _DISPLAY

#define DSP_BOX
//#define DSP_DEBUG
// #define DSP_THREAD

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#ifdef DSP_THREAD
#include <pthread.h>
#endif
#include "./bit_oper.h"
#include "./common.h"


#define DSP_XMAX_SIZE 10
#define DSP_YMAX_SIZE 2


#ifdef DSP_BOX
	#define DSP_XBOX_SIZE ( DSP_XMAX_SIZE + 2 )
	#define DSP_YBOX_SIZE ( DSP_YMAX_SIZE + 2 )
#endif

#define DSP_POSX_MIN  0
#define DSP_POSY_MIN  0
#define DSP_POSX_MAX  ( DSP_XMAX_SIZE - 1 )
#define DSP_POSY_MAX  ( DSP_YMAX_SIZE - 1 )

#define fContentChanged	0

typedef struct {
	curscoord	x_pos;
	curscoord	y_pos;
	curscoord	x_max;
	curscoord	y_max;	
	WINDOW  *	wnd;
#ifdef DSP_THREAD
	char		status;
	pthread_t 	th;
	int         rc;
#endif	
} t_display;

t_display *p_display;

#ifndef DSP_THREAD
void dspRefresh();
#endif

void dspInit();

void dspWrite( char dc );

#endif
