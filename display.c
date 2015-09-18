
#include "./display.h"
short int dspXmax() {
	return ( p_display->x_max );
}

short int dspYmax() {
	return ( p_display->y_max );
}

short int dspXget() {
	return ( p_display->x_pos );
}

void dspXset( short int p ) {
	p_display->x_pos = p;
}

short int dspYget() {
	return ( p_display->y_pos );
}

void dspYset( short int p ) {
	p_display->y_pos = p;
}

void dspIncY() {
	p_display->y_pos++;
	if ( p_display->y_pos == DSP_YMAX_SIZE ) {
		p_display->y_pos = DSP_POSY_MAX; 
		return;		
	}
}

void dspIncX() {
	p_display->x_pos++;
	if ( p_display->x_pos == DSP_XMAX_SIZE ) {
		if ( p_display->y_pos < DSP_POSY_MAX ) {
			p_display->x_pos = DSP_POSX_MIN;
			dspIncY();
		} else {
			p_display->x_pos = DSP_POSX_MAX;
		}
	}
}

void dspDecY() {
	p_display->y_pos--;
	if ( p_display->y_pos < DSP_POSY_MIN ) {
		p_display->y_pos = DSP_POSY_MIN;
	}
}

void dspDecX() {
	p_display->x_pos--;
	if ( p_display->x_pos < DSP_POSX_MIN ) {
		if ( p_display->y_pos > DSP_POSY_MIN ) {
			p_display->x_pos = DSP_POSX_MAX;
			dspDecY();
		} else {
			p_display->x_pos = DSP_POSX_MIN;
		}
	}
}

void dspCursorMove() {
#ifdef DSP_BOX
	wmove( p_display->wnd, p_display->y_pos+1, p_display->x_pos+1 );
#else
	wmove( p_display->wnd, p_display->y_pos, p_display->x_pos );
#endif
}

#ifdef DSP_THREAD
void *_dspRefresh( void *param ) {
	t_display *_display;
	_display = (t_display *) param;
	
	while ( 1 ) {
		if ( _display->status && 0b00000001 != 0 ) {
			_display->status &= 0b11111110;
			refresh(); // curses call to update screen
		}
	}

	pthread_exit(NULL);
}
#endif
#ifndef DSP_THREAD
void dspRefresh() {
	refresh(); // curses call to update screen
}
#endif

void dspCursorHome() {
	p_display->x_pos = 0;
	p_display->y_pos = 0;
	dspCursorMove();
}

void dspClear() {
	wclear( p_display->wnd );
#ifdef DSP_BOX
	box( p_display->wnd, ACS_VLINE, ACS_HLINE );
#endif
	dspCursorHome();
}

void dspInit() {
	p_display	= malloc( sizeof( t_display ) );
	
	// p_display->x_pos = 0;
	// p_display->y_pos = 0;
	p_display->x_max = DSP_XMAX_SIZE;
	p_display->y_max = DSP_YMAX_SIZE;

	p_display->wnd 	= initscr();
	
#ifdef DSP_BOX
	wresize( p_display->wnd, DSP_YBOX_SIZE, DSP_XBOX_SIZE );
	// wclear( p_display->wnd );
	// box( p_display->wnd, ACS_VLINE, ACS_HLINE );
#else
	wresize( p_display->wnd, DSP_YMAX_SIZE, DSP_XMAX_SIZE );
	// wclear( p_display->wnd );
#endif

	dspClear();
	
#ifdef DSP_DEBUG
	printf("\n\rWND: %08X\n\r", p_display->wnd);
#endif

	dspCursorHome();

	cbreak(); // curses call to set no waiting for Enter key
	noecho(); // curses call to set no echoing
	
#ifndef DSP_THREAD
	refresh(); // curses call to update screen
#else
	p_display->status &= 0b11111110;
	p_display->rc = pthread_create(&p_display->th, NULL, _dspRefresh, (void *)p_display);
	if ( p_display->rc ) {
		printf("error creating thread.");
	}	
#endif
}

void dspWrite( char dc ) {
 
	if ( dc >= 32 && dc != 127 ) {
		wdelch( p_display->wnd ); 
		winsch( p_display->wnd, dc ); // curses calls to replace character under cursor by dc

		dspIncX();
		dspCursorMove();
	} else {
		switch ( dc ) {
			case 127:
				dspDecX();
				dspCursorMove();

				wdelch( p_display->wnd ); 
				winsch( p_display->wnd, ' ' ); // curses calls to replace character under cursor by dc
				break;
			default:
				printf("%02X", dc);
				break;
		}
	}
#ifndef DSP_THREAD
	refresh(); // curses call to update screen+
#else
	p_display->status |= 0b00000001;
#endif
}

