#include <stdio.h>
#include "screen.h"

#include <pthread.h>
#include <windows.h>

#include "./common.h"

#include "./registry.h"
#include "./screen.h"
#include "./keyboard.h"

#ifndef _KEYBOARD
#ifdef __MINGW32__
#include <ncursest/ncurses.h>
#else
#include <curses.h>
#endif
#endif

#define MEMLOAD(mt,t,p,i) p = (t*) mt; mt = (uchar *) p + sizeof(t); i;
#define MEMINIT(mt,me) mt = me
#define DUMPMEM(s,mb,t,p) DumpMem( s, (int) ((uchar*)p-(uchar*)mb), (int) ((uchar*)p-(uchar*)mb+sizeof(t)-1) )	

uchar memory[ 4096 ];
uchar *mem_top;

void MemInfo() {

	#ifdef DSP_DEBUG_WIN
	
	mvwprintw(p_debug->wMain, p_debug->y_pos, p_debug->x_pos, "MemInfo:");
	p_debug->y_pos++;
	mvwprintw(p_debug->wMain, p_debug->y_pos, p_debug->x_pos, "BaseMem   %08X (%4d) Bytes", memory, sizeof(memory));
	#ifdef _REGISTRY	
	p_debug->y_pos++;
	mvwprintw(p_debug->wMain, p_debug->y_pos, p_debug->x_pos, "RegMem    %08X (%4d) Bytes", p_registry, sizeof(t_registry) );
	#endif	
	#ifdef _KEYBOARD	
	p_debug->y_pos++;
	mvwprintw(p_debug->wMain, p_debug->y_pos, p_debug->x_pos, "KbMem     %08X (%4d) Bytes", p_keyboard, sizeof(t_keyboard) );
	#endif	
	#ifdef _SCREEN	
	p_debug->y_pos++;
	mvwprintw(p_debug->wMain, p_debug->y_pos, p_debug->x_pos, "ScreenMem %08X (%4d) Bytes", p_screen, sizeof(t_screen) );
	#endif	
	
	#else
	
	/*
	printf("\n\r");
	printf("\n\rMemInfo:\n\r");
	printf( "BaseMem   %08X (%4d) Bytes\n\r", memory, sizeof(memory) );
	printf( "TopMem    %08X (%4d) Bytes\n\r", mem_top, mem_top-memory );
	
	#ifdef _REGISTRY	
	printf( "RegMem    %08X (%4d) Bytes\n\r", p_registry, sizeof(t_registry) );
	#endif	

	#ifdef _KEYBOARD	
	printf( "KbMem     %08X (%4d) Bytes\n\r", p_keyboard, sizeof(t_keyboard) );
	#endif	

	#ifdef _SCREEN	
	printf( "ScreenMem %08X (%4d) Bytes\n\r", p_screen, sizeof(t_screen) );
	#endif	
	*/
		
	#endif	
	
}

void DumpMem( char* s, int x_from, int x_to ) {
	int x,i;

	#ifdef DSP_DEBUG_WIN
	
	p_debug->x_pos = 1;
	p_debug->y_pos++;
	p_debug->y_pos++;
	mvwprintw(p_debug->wMain, p_debug->y_pos, p_debug->x_pos, "Dump %s ( %08X ):", s, &memory[x_from]);
	i=0;
	p_debug->x_pos = 1;
	p_debug->y_pos++;
	for ( x=x_from; x<=x_to; x++ ) {
		if ( i>31 ) {
			p_debug->x_pos = 1;
			p_debug->y_pos++;
			i=0;
		}
		if ( i==0 ) {
			mvwprintw(p_debug->wMain, p_debug->y_pos, p_debug->x_pos, "%04X : ", x );
			p_debug->x_pos += 7;
		}
		mvwprintw(p_debug->wMain, p_debug->y_pos, p_debug->x_pos, "%02X ", memory[x] );
		p_debug->x_pos += 3;
		i++;
	}
	
	#else

/*	
	printf("\n\r");
	printf( "\n\rDump ( %08X ):\n\r", &memory[x_from] );
	i=0;
	for ( x=x_from; x<=x_to; x++ ) {
		if ( i>15 ) {
			printf("\n\r");
			i=0;
		}
		if ( i==0 ) {
			printf( "%04X : ", x );
		}
		printf( "%02X ", memory[x] );
		i++;
	}
	printf("\n\r");
*/

	#endif	

}

void DoScreenRepaint() {

	scrRepaint();

	#ifdef DSP_DEBUG_WIN
	
	p_debug->x_pos = 1;
	p_debug->y_pos = 1;
	
	MemInfo();
	
	#ifdef _REGISTRY	
	DUMPMEM("REG: ", memory,t_registry,p_registry);
	#endif
	
	#ifdef _KEYBOARD	
	DUMPMEM("KBD: ", memory,t_keyboard,p_keyboard);
	#endif
	
	#ifdef _SCREEN	
	DUMPMEM("SCR: ", memory,t_screen,p_screen);
	#endif

	dspInfo( "Frame:   ", p_debug,  p_frame );
	dspInfo( "Display: ", p_debug,  p_display );
	dspInfo( "Debug:   ", p_debug,  p_debug );
	
	wrefresh( p_debug->wMain );
	
	#else

	/*
	p_frame->y_pos = p_display->y_max + 2;
	dspInfo( "Frame:   ", p_frame,  p_frame );
	dspInfo( "Display: ", p_frame,  p_display );
	
	wrefresh( p_frame->wMain );
	*/
	
	#endif

}

void Init() {

	MEMINIT(mem_top,memory);
	
	#ifdef _REGISTRY	
	MEMLOAD(mem_top,t_registry,p_registry,RegistryInit());
	#endif	
	
	#ifdef _KEYBOARD	
	MEMLOAD(mem_top,t_keyboard,p_keyboard,kbInit());
	#else
	cbreak(); // curses call to set no waiting for Enter key
	noecho(); // curses call to set no echoing
	#endif	

	#ifdef _SCREEN	
	MEMLOAD(mem_top,t_screen,p_screen,scrInit( DoScreenRepaint ));
	//MEMLOAD(mem_top,t_screen,p_screen,scrInit( scrRepaint ));
	//MEMLOAD(mem_top,t_screen,p_screen,scrInit( NULL ));
	// if ( pthread_create( &thScreenRepaint, NULL, th_DoScreenRepaint, (void *)memory ) ) {
	// if ( pthread_create( &thScreenRepaint, NULL, th_DoScreenRepaint, (void *)DoScreenRepaint ) ) {
	//if ( pthread_create( &thScreenRepaint, NULL, th_DoScreenRepaint, NULL ) ) {
	//scrCreateThread( DoScreenRepaint );
	#endif	
	
}

int main( int argc, char *argv[] ) {

	#ifndef _KEYBOARD
	uchar c = '\0';
	#endif
	
	Init();
	
	// DoScreenRepaint();
	// printf( "\nGO" );
	// sleepMs( 2000 );
	// #ifdef _KEYBOARD	
	// DumpMem( (int) ((unsigned char*)p_keyboard-(unsigned char*)memory), (int) ((unsigned char*)p_keyboard-(unsigned char*)memory+sizeof(t_keyboard)-1) );
	// #endif
	// printf( "\nkbGetCh %02X", kbGetCh() );
	// #ifdef _KEYBOARD	
	// DumpMem( (int) ((unsigned char*)p_keyboard-(unsigned char*)memory), (int) ((unsigned char*)p_keyboard-(unsigned char*)memory+sizeof(t_keyboard)-1) );
	// #endif
	// return 0;
	
	while (1) {
	
		#ifdef _KEYBOARD
			#ifndef THREAD_KEYBOARD	
				kbPutCh();
			#endif
			scrWrite( kbGetCh() );
		#else
			c = getch();
			scrWrite( c );
		#endif
		
		#ifndef THREAD_SCREEN_REPAINT
		DoScreenRepaint();
		#endif
		
		// if ( TSTBIT( p_keyboard->status , KB_BUFFER_EMPTY ) == 0 ) {
			// printf("\n\r" );
			// #ifdef _KEYBOARD	
			// DumpMem( (int) ((unsigned char*)p_keyboard-(unsigned char*)memory), (int) ((unsigned char*)p_keyboard-(unsigned char*)memory+sizeof(t_keyboard)-1) );
			// #endif
			// printf("\n\r c: %02X", kbGetCh() );
		// }
		
		sleepMs( 100 );
	}
	
	return 0;
}