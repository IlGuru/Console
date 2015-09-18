#include <stdio.h>
#include "./screen.h"

//----------------------------------------------------
//	Screen

// void _clrscr() {
	// const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
	// printf( CLEAR_SCREE_ANSI );
// }

//----------------------------------------------------

void scrClear() {
	scrMem->x_pos = 0;
	scrMem->y_pos = 0;
	// _clrscr();
}

//----------------------------------------------------

void scrClearMemBuf() {
	int xg = 0;
	for (xg = 0 ; xg < SCR_XMAX_SIZE*SCR_YMAX_SIZE ; xg++) {
		scrMem->Buffer[xg] = '+';
	}
}

//----------------------------------------------------

void scrInit() {

	dsp_init();

	scrMem->x_max		= SCR_XMAX_SIZE;
	scrMem->y_max		= SCR_YMAX_SIZE;
	// scrMem->BufSize		= SCR_XMAX_SIZE*SCR_YMAX_SIZE;
	scrClearMemBuf();
}

//----------------------------------------------------

void scrInc_xpos() {

	scrMem->x_pos++;

	if ( scrMem->x_pos >= SCR_XMAX_SIZE ) {
		scrMem->x_pos = 0;
		scrInc_ypos();
	}		

}

void scrInc_ypos() {

	scrMem->y_pos++;

	if ( scrMem->y_pos >= SCR_YMAX_SIZE ) {
		scrMem->x_pos = SCR_XMAX_SIZE;
		scrMem->y_pos = SCR_YMAX_SIZE;
	}		

}

void scrWrite( unsigned char c ) {
	
	if ( scrMem->y_pos < SCR_YMAX_SIZE ) {
		// if ( c == '\0' ) c = ' ';
		// if ( c == '\n' ) scrMem->y_pos++;
		// if ( c == '\r' ) scrMem->x_pos = 0;

		scrMem->Buffer[ scrMem->y_pos*SCR_XMAX_SIZE + scrMem->x_pos ] = c;
		scrInc_xpos();
		// putchar( c );		
	} else {
		// printf( "%02X ", ((int) c) );
		return;
	}		

}

unsigned char scrRead() {
	return ( scrMem->Buffer[ scrMem->y_pos*SCR_XMAX_SIZE + scrMem->x_pos ] );
}

//----------------------------------------------------

void scrRepaint(int amount) {
	// int xg = 0;
	
	// scrClear();
		
	// for (xg = 0 ; xg < amount ; xg++) {
		// scrWrite(scrMem->Buffer[xg]);		
	// }
	short int xs, xe, ys, ye, x, y;
	
	xs = ( scrMem->x_pos < dsp_xmax() ? 0 : scrMem->x_pos - dsp_xmax()+1 );
	xe = xs + dsp_xmax();
	ys = ( scrMem->y_pos < dsp_ymax() ? 0 : scrMem->y_pos - dsp_ymax()+1 );
	ye = ys + dsp_ymax();
	
	dsp_xset( 0 );
	dsp_yset( 0 );
	for ( y = ys; y < ye; y++ ) {
		for ( x = xs; x < xe; x++ ) {
			// printf("%d->%d ", y*SCR_XMAX_SIZE + x, dsp_yget()*dsp_xmax()+dsp_xget() );
			if ( x-xs < SCR_XMAX_SIZE && y-ys < SCR_YMAX_SIZE ) {
				dsp_write( scrMem->Buffer[ y*SCR_XMAX_SIZE + x ] );
			} else {
				dsp_write( '~' );
			}
		}
	}
	
	dsp_repaint();

	// printf("\nx_pos:%d y_pos:%d x_max:%d y_max:%d\n", scrMem->x_pos, scrMem->y_pos, scrMem->x_max, scrMem->y_max);
	// printf("\ndsp_xget:%d dsp_yget:%d dsp_xmax:%d dsp_ymax:%d\n", dsp_xget(), dsp_yget(), dsp_xmax(), dsp_ymax());
	// printf("\nxs:%d xe:%d ys:%d ye:%d\n", xs, xe, ys, ye);
	
}
