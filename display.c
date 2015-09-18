#include "./display.h"

//----------------------------------------------------
//	Display

//----------------------------------------------------

curscoord dspXmax( t_display *p_dest ) {
	return ( p_dest->x_max );
}

curscoord dspYmax( t_display *p_dest ) {
	return ( p_dest->y_max );
}
/*
curscoord dspXget( t_display *p_dest ) {
	return ( p_dest->x_pos );
}

void dspXset( t_display *p_dest, curscoord p ) {
	p_dest->x_pos = p;
}

curscoord dspYget( t_display *p_dest ) {
	return ( p_dest->y_pos );
}

void dspYset( t_display *p_dest, curscoord p ) {
	p_dest->y_pos = p;
}
*/

void dspIncY( t_display *p_dest ) {
	p_dest->y_pos++;
	if ( p_dest->y_pos == p_dest->x_max ) {
		p_dest->y_pos = (p_dest->y_max - 1); 
		return;		
	}
}

void dspIncX( t_display *p_dest ) {
	p_dest->x_pos++;
	if ( p_dest->x_pos == p_dest->x_max ) {
		if ( p_dest->y_pos < (p_dest->y_max - 1) ) {
			p_dest->x_pos = 0;
			dspIncY( p_dest );
		} else {
			p_dest->x_pos = (p_dest->x_max - 1);
		}
	}
}

void dspDecY( t_display *p_dest ) {
	if ( p_dest->y_pos == DSP_POSY_MIN ) {
		p_dest->y_pos = DSP_POSY_MIN;
	} else {
		p_dest->y_pos--;
	}
}

void dspDecX( t_display *p_dest ) {
	if ( p_dest->x_pos == DSP_POSX_MIN ) {
		if ( p_dest->y_pos > DSP_POSY_MIN ) {
			p_dest->x_pos = (p_dest->x_max - 1);
			dspDecY( p_dest );
		} else {
			p_dest->x_pos = DSP_POSX_MIN;
		}
	} else {
		p_dest->x_pos--;
	}
}

void dspCursorMove( t_display *p_dest ) {
#ifdef DSP_BOX
	wmove( p_dest->wMain, p_dest->y_pos+1, p_dest->x_pos+1 );
#else
	wmove( p_dest->wMain, p_dest->y_pos, p_dest->x_pos );
#endif
}

void dspRefresh( t_display *p_dest ) {
	wrefresh( p_dest->wMain );
	CLRBIT(p_dest->status,f_dspContentChanged);
}

#ifdef THREAD_DISPLAY_REPAINT
void *th_dspRefresh( void *param ) {
	FNINPUT fCallBack;
	fCallBack = (FNINPUT)(param);
	
	while ( 1 ) {
		if ( TSTBIT(p_display->status,f_dspContentChanged) ) {
			if ( fCallBack != NULL ) {
				fCallBack();
			} else {
				dspRefresh( p_display );
			}
			#if DSP_MS_REFRESH > 0
			sleepMs( DSP_MS_REFRESH );
			#endif
		}
	}

	pthread_exit(NULL);
}
#endif

void dspCursorHome( t_display *p_dest ) {
	p_dest->x_pos = 0;
	p_dest->y_pos = 0;
	dspCursorMove( p_dest );
}

void dspClear( t_display *p_dest ) {
	wclear( p_dest->wMain );
	#ifdef DSP_BOX
	box( p_dest->wMain, ACS_VLINE, ACS_HLINE );
	#endif
	dspCursorHome( p_dest );
}

t_display* dspInitDisplay( t_display *p_parent, curscoord x_dim, curscoord y_dim, curscoord x_st, curscoord y_st ) {
	
	t_display *p_dest;
	
	p_dest  	= malloc( sizeof( t_display ) );
	
	if ( p_parent == NULL ) {
		
		p_dest->wMain 		= initscr();
		noecho();
		curs_set(FALSE);
		#ifdef DSP_BOX
		getmaxyx(stdscr, p_dest->y_box, p_dest->x_box);
		p_dest->y_max = p_dest->y_box - 2;
		p_dest->x_max = p_dest->x_box - 2;
		#else
		getmaxyx(stdscr, p_dest->y_max, p_dest->x_max);
		p_dest->y_max = p_dest->y_max;
		p_dest->x_max = p_dest->x_max;
		#endif
		
	} else {

		#ifdef DSP_BOX
		p_dest->x_box = x_dim;
		p_dest->y_box = y_dim;
		p_dest->x_max = x_dim - 2;
		p_dest->y_max = y_dim - 2;
		#else
		p_dest->x_max = x_dim;
		p_dest->y_max = y_dim;
		#endif

		#ifdef DSP_BOX
		p_dest->wMain = subwin(p_parent->wMain, p_dest->y_box,	p_dest->x_box, y_st,	x_st);
		#else
		p_dest->wMain = subwin(p_parent->wMain, p_dest->y_max, 	p_dest->x_max, y_st,	x_st);
		#endif

		#ifdef DSP_BOX
		box( p_dest->wMain, ACS_VLINE, ACS_HLINE );
		#endif
		
	}

	dspClear( p_dest );

	dspRefresh( p_dest );

	CLRBIT( p_dest->status,f_dspContentChanged );
	
	return p_dest;
}

void dspInit() {

	p_frame = dspInitDisplay( NULL, 0, 0, 0, 0 );
	#ifdef DSP_BOX
	p_display = dspInitDisplay( p_frame, DSP_XBOX_SIZE,	DSP_YBOX_SIZE, 	DSP_XY_OFFSET, DSP_XY_OFFSET );
	#else
	p_display = dspInitDisplay( p_frame, DSP_XMAX_SIZE,	DSP_YMAX_SIZE, 	DSP_XY_OFFSET, DSP_XY_OFFSET );
	#endif
#ifdef DSP_DEBUG_WIN
	#ifdef DSP_BOX
	p_debug = dspInitDisplay( p_frame, (p_frame->x_box-2*(DSP_XY_OFFSET)), (p_frame->y_box-p_display->y_box-2*(DSP_XY_OFFSET)), DSP_XY_OFFSET, 	p_display->y_box+DSP_XY_OFFSET );
	#else
	p_debug = dspInitDisplay( p_frame, (p_frame->x_max-2*(DSP_XY_OFFSET)), (p_frame->y_max-p_display->y_max-2*(DSP_XY_OFFSET)), DSP_XY_OFFSET, 	p_display->y_max+DSP_XY_OFFSET );
	#endif
#endif

#ifdef THREAD_DISPLAY_REPAINT
	if ( pthread_create(&p_display->th, NULL, th_dspRefresh, NULL) ) {
		printf("error creating thread.");
	}	
#endif

/*	
	p_frame  	= malloc( sizeof( t_display ) );
	p_display	= malloc( sizeof( t_display ) );
#ifdef DSP_DEBUG_WIN
	p_debug 	= malloc( sizeof( t_display ) );
#endif
	
	p_frame->wMain 		= initscr();
	noecho();
	curs_set(FALSE);
	#ifdef DSP_BOX
	getmaxyx(stdscr, p_frame->y_box, p_frame->x_box);
	p_frame->y_max = p_frame->y_box - 2;
	p_frame->x_max = p_frame->x_box - 2;
	#else
	getmaxyx(stdscr, p_frame->y_max, p_frame->x_max);
	p_frame->y_max = p_frame->y_max;
	p_frame->x_max = p_frame->x_max;
	#endif
	
	#ifdef DSP_BOX
	p_display->x_box = DSP_XBOX_SIZE;
	p_display->y_box = DSP_YBOX_SIZE;
	p_display->x_max = DSP_XMAX_SIZE;
	p_display->y_max = DSP_YMAX_SIZE;
	#ifdef DSP_DEBUG_WIN
	p_debug->x_box   = p_frame->x_box - 2*(DSP_XY_OFFSET);
	p_debug->y_box   = p_frame->y_box - p_display->y_box - 2*(DSP_XY_OFFSET);
	p_debug->x_max   = p_debug->x_box - 2;
	p_debug->y_max   = p_debug->y_box - 2;
	#endif
	#else
	p_display->x_max = DSP_XMAX_SIZE;
	p_display->y_max = DSP_YMAX_SIZE;
	#ifdef DSP_DEBUG_WIN
	p_debug->x_max   = p_frame->x_max - 2*(DSP_XY_OFFSET);
	p_debug->y_max   = p_frame->y_max - p_display->y_max - 2*(DSP_XY_OFFSET);
	#endif
	#endif

	#ifdef DSP_BOX
	p_display->wMain = subwin(p_frame->wMain, p_display->y_box,		p_display->x_box, 	DSP_XY_OFFSET,						DSP_XY_OFFSET);
	#ifdef DSP_DEBUG_WIN
	p_debug->wMain   = subwin(p_frame->wMain, p_debug->y_box, 		p_debug->x_box,		p_display->y_box+DSP_XY_OFFSET, 	DSP_XY_OFFSET);
	#endif
	#else
	p_display->wMain = subwin(p_frame->wMain, p_display->y_max, 	p_display->x_max, 	DSP_XY_OFFSET,						DSP_XY_OFFSET);
	#ifdef DSP_DEBUG_WIN
	p_debug->wMain   = subwin(p_frame->wMain, p_debug->y_max, 		p_debug->x_max, 	p_display->y_max+DSP_XY_OFFSET, 	DSP_XY_OFFSET);
	#endif
	#endif

	#ifdef DSP_BOX
	box( p_frame->wMain, ACS_VLINE, ACS_HLINE );
	box( p_display->wMain, ACS_VLINE, ACS_HLINE );
	#ifdef DSP_DEBUG_WIN
	box( p_debug->wMain, ACS_VLINE, ACS_HLINE );
	#endif
	#endif

	dspClear( p_display );
	#ifdef DSP_DEBUG_WIN
	dspClear( p_debug );
	#endif

	CLRBIT(p_display->status,f_dspContentChanged);
	
#ifndef THREAD_DISPLAY_REPAINT
	dspRefresh( p_display );
	#ifdef DSP_DEBUG_WIN
	dspRefresh( p_debug );
	#endif
#else
	if ( pthread_create(&p_display->th, NULL, th_dspRefresh, NULL) ) {
		printf("error creating thread.");
	}	
#endif
*/
}

void dspWrite( t_display *p_dest, char dc ) {
 
	while ( TSTBIT(p_dest->status,f_dspRepainting) ) {
		;
	}
	
	SETBIT(p_dest->status,f_dspRepainting);
	
	if ( dc >= 32 && dc != 127 ) {
		wdelch( p_dest->wMain ); 
		winsch( p_dest->wMain, dc ); // curses calls to replace character under cursor by dc

		dspIncX( p_dest );
		dspCursorMove( p_dest );
	} else {
		switch ( dc ) {
			case 127:
				dspDecX( p_dest );
				dspCursorMove( p_dest );

				wdelch( p_dest->wMain ); 
				winsch( p_dest->wMain, ' ' ); // curses calls to replace character under cursor by dc
				break;
			default:
				printf("%02X", dc);
				break;
		}
	}

	SETBIT(p_dest->status,f_dspContentChanged);

	CLRBIT(p_dest->status,f_dspRepainting);
	
}

void dspInfo( char* s, t_display *p_dest,  t_display *p_info ) {
	
	p_dest->x_pos=1;
	p_dest->y_pos++;
	#ifdef DSP_BOX
	mvwprintw(p_dest->wMain, p_dest->y_pos, p_dest->x_pos, "%s : xb:%3d yb:%3d xm:%3d ym:%3d ", s, p_info->x_box, p_info->y_box, p_info->x_max, p_info->y_max );
	#else
	mvwprintw(p_dest->wMain, p_dest->y_pos, p_dest->x_pos, "%s : xm:%3d ym:%3d ", s, p_info->x_max, p_info->y_max );
	#endif
}