#include "./display.h"

//----------------------------------------------------
//	Display

void _clrdsp() {
	const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
	printf( CLEAR_SCREE_ANSI );
}

//----------------------------------------------------
void dsp_clear() {
	_clrdsp();
	dspMem->x_pos = 0;
	dspMem->y_pos = 0;
}

void dsp_init() {

	dspMem = (t_dspMem *) malloc( sizeof( t_dspMem ) );

	for (dspMem->y_pos = 0 ; dspMem->y_pos < DSP_YMAX_SIZE ; dspMem->y_pos++) {
		for (dspMem->x_pos = 0 ; dspMem->x_pos < DSP_XMAX_SIZE ; dspMem->x_pos++) {
			dspMem->Buffer[ dspMem->y_pos*DSP_XMAX_SIZE + dspMem->x_pos ] = ' ';		
		}
	}
	
	dspMem->x_max		= DSP_XMAX_SIZE;
	dspMem->y_max		= DSP_YMAX_SIZE;

	dsp_clear();
}

//----------------------------------------------------

void dsp_inc_xpos() {

	dspMem->x_pos++;

	if ( dspMem->x_pos >= DSP_XMAX_SIZE ) {
		dspMem->x_pos = 0;
		dsp_inc_ypos();
	}		

}

void dsp_inc_ypos() {

	dspMem->y_pos++;

	if ( dspMem->y_pos >= DSP_YMAX_SIZE ) {
		return;
	}		

}

short int dsp_xmax() {
	return ( dspMem->x_max );
}

short int dsp_ymax() {
	return ( dspMem->y_max );
}

short int dsp_xget() {
	return ( dspMem->x_pos );
}

void dsp_xset( short int p ) {
	dspMem->x_pos = p;
}

short int dsp_yget() {
	return ( dspMem->y_pos );
}

void dsp_yset( short int p ) {
	dspMem->y_pos = p;
}

unsigned char dsp_read() {
	return ( dspMem->Buffer[ dspMem->y_pos*DSP_XMAX_SIZE + dspMem->x_pos ] );
}

void dsp_write( unsigned char c ) {

	if ( dspMem->y_pos < DSP_YMAX_SIZE ) {
		dspMem->Buffer[ dspMem->y_pos*DSP_XMAX_SIZE + dspMem->x_pos ] = c;
		dsp_inc_xpos();
	} else {
		return;
	}		

}

//----------------------------------------------------

void dsp_print( unsigned char c ) {
	if ( c >= 32 )
		putchar( c );		
}

void dsp_repaint() {
	
	dsp_clear();
		
	for (dspMem->y_pos = 0 ; dspMem->y_pos < DSP_YMAX_SIZE ; dspMem->y_pos++) {
		for (dspMem->x_pos = 0 ; dspMem->x_pos <= DSP_XMAX_SIZE ; dspMem->x_pos++) {
			if ( dspMem->x_pos < DSP_XMAX_SIZE ) {
			// printf("%d:%c ", dspMem->y_pos*DSP_XMAX_SIZE + dspMem->x_pos, dspMem->Buffer[ dspMem->y_pos*DSP_XMAX_SIZE + dspMem->x_pos ] );
			// printf("%d,%d ", dspMem->x_pos, dspMem->y_pos);
				dsp_print( dspMem->Buffer[ dspMem->y_pos*DSP_XMAX_SIZE + dspMem->x_pos ] );	
			} else {
			// if ( dspMem->x_pos == DSP_XMAX_SIZE ) {
				putchar( '\n' );
				putchar( '\r' );
			}
		}
	}

}
