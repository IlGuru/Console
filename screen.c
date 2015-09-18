#include <stdio.h>
#include "./screen.h"

//----------------------------------------------------
//	Screen

void scrMem_ClearBuf() {
	int xg = 0;
	for (xg = 0 ; xg < scr_xmax*scr_ymax ; xg++) {
		scrMem->Buffer[xg] = '\0';
	}
}

void scrMem_Init() {
	scrMem->x_max		= scr_xmax;
	scrMem->y_max		= scr_ymax;
	scrMem->BufSize		= scr_xmax*scr_ymax;
	scrMem_ClearBuf();
}

//----------------------------------------------------

void clrscr() {
	const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
	printf( CLEAR_SCREE_ANSI );
}

//----------------------------------------------------
void scr_clear() {
	scrMem->x_pos = 0;
	scrMem->y_pos = 0;
	clrscr();
}

void scr_write( char c ) {

	if ( scrMem->x_pos >= scr_xmax ) {
		scrMem->x_pos = 0;
		scrMem->y_pos++;
		putchar('\n');
		putchar('\r');
	}		
	if ( scrMem->y_pos < scr_ymax ) {
		if ( c == '\0' ) c = ' ';
		if ( c == '\n' ) scrMem->y_pos++;
		if ( c == '\r' ) scrMem->x_pos = 0;

		scrMem->x_pos++;
		putchar( c );		
	} else {
		printf( "%02X ", ((int) c) );
	}		

}

//----------------------------------------------------
void scr_Repaint(int amount) {
	int xg = 0;
	
	scr_clear();
		
	for (xg = 0 ; xg < amount ; xg++) {
		scr_write(scrMem->Buffer[xg]);		
	}

}
