#include <stdio.h>
#include "./screen.h"

//----------------------------------------------------
//	Screen

//----------------------------------------------------

void scrCursorHome() {
	p_screen->x_pos = 0;
	p_screen->y_pos = 0;
}

//----------------------------------------------------

void scrClearMemBuf() {
	int xg = 0;
	for (xg = 0 ; xg < SCR_XMAX_SIZE*SCR_YMAX_SIZE ; xg++) {
		p_screen->Buffer[xg] = ' ';
	}
}

//----------------------------------------------------

void scrInit() {

	dspInit();

	p_screen->x_max		= SCR_XMAX_SIZE;
	p_screen->y_max		= SCR_YMAX_SIZE;
	
	scrClearMemBuf();

	scrRepaint(0);
	
}

//----------------------------------------------------

void scrIncX() {

	p_screen->x_pos++;

	if ( p_screen->x_pos >= SCR_XMAX_SIZE ) {
		if ( p_screen->y_pos < SCR_POSY_MAX ) {
			p_screen->x_pos = SCR_POSX_MIN;
			scrIncY();
		} else {
			p_screen->x_pos = SCR_POSX_MAX;
		}
	}		

}

void scrIncY() {

	p_screen->y_pos++;

	if ( p_screen->y_pos == SCR_YMAX_SIZE ) {
		p_screen->y_pos = SCR_POSY_MAX; 
		return;		
	}
}

void scrDecY() {
	p_screen->y_pos--;
	if ( p_screen->y_pos < SCR_POSY_MIN ) {
		p_screen->y_pos = SCR_POSY_MIN;
	}
}

void scrDecX() {
	p_screen->x_pos--;
	if ( p_screen->x_pos < SCR_POSX_MIN ) {
		if ( p_screen->y_pos > SCR_POSY_MIN ) {
			p_screen->x_pos = SCR_POSX_MAX;
			scrDecY();
		} else {
			p_screen->x_pos = SCR_POSX_MIN;
		}
	}
}

void scrWrite( unsigned char c ) {
	
	if ( p_screen->y_pos < SCR_YMAX_SIZE ) {
		// if ( c == '\0' ) c = ' ';
		// if ( c == '\n' ) p_screen->y_pos++;
		// if ( c == '\r' ) p_screen->x_pos = 0;

		p_screen->Buffer[ p_screen->y_pos*SCR_XMAX_SIZE + p_screen->x_pos ] = c;
		scrIncX();
		// putchar( c );		
	} else {
		// printf( "%02X ", ((int) c) );
		return;
	}		

}

unsigned char scrRead() {
	return ( p_screen->Buffer[ p_screen->y_pos*SCR_XMAX_SIZE + p_screen->x_pos ] );
}

//----------------------------------------------------

void scrRepaint(int amount) {

	short int xs, xe, ys, ye, x, y;
	
	xs = ( p_screen->x_pos < dspYmax() ? 0 : p_screen->x_pos - dspXmax()+1 );
	xe = xs + dspXmax();
	ys = ( p_screen->y_pos < dspYmax() ? 0 : p_screen->y_pos - dspYmax()+1 );
	ye = ys + dspYmax();
	
	dspCursorHome();
	for ( y = ys; y < ye; y++ ) {
		for ( x = xs; x < xe; x++ ) {
			if ( x-xs < SCR_XMAX_SIZE && y-ys < SCR_YMAX_SIZE ) {
				dspWrite( p_screen->Buffer[ y*SCR_XMAX_SIZE + x ] );
			} else {
				dspWrite( '~' );
			}
		}
	}
	
}
