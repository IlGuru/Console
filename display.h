#ifndef _DISPLAY

#define _DISPLAY

#define DSP_BOX
// #define THREAD_DISPLAY_REPAINT
#define DSP_DEBUG_WIN

#include <stdio.h>
#include <stdlib.h>

#ifdef __MINGW32__
#include <ncursest/ncurses.h>
#else
#include <curses.h>
#endif

#ifdef THREAD_DISPLAY_REPAINT
#include <pthread.h>
#endif

#include <string.h>

#include "./common.h"
#include "./bit_oper.h"


#define DSP_XMAX_SIZE 40
#define DSP_YMAX_SIZE 15


#ifdef DSP_BOX
	#define DSP_XBOX_SIZE ( DSP_XMAX_SIZE + 2 )
	#define DSP_YBOX_SIZE ( DSP_YMAX_SIZE + 2 )
#endif

#define DSP_POSX_MIN  0
#define DSP_POSY_MIN  0

#define DSP_XY_OFFSET  1

#define DSP_MS_REFRESH		0

#define f_dspContentChanged	0
#define f_dspRepainting		1

typedef struct {
	WINDOW  *	wMain;
	curscoord	x_pos;
	curscoord	y_pos;
	curscoord	x_max;
	curscoord	y_max;	
#ifdef DSP_BOX	
	curscoord	x_box;
	curscoord	y_box;	
#endif
#ifdef THREAD_DISPLAY_REPAINT
	pthread_t 	th;
#endif	
	char		status;
} t_display;

t_display *p_frame;
t_display *p_display;
#ifdef DSP_DEBUG_WIN
t_display *p_debug;
#endif

#ifndef THREAD_DISPLAY_REPAINT
void dspRefresh();
#endif

void dspInit();

void dspWrite( t_display *p_dest, char dc );

#endif
