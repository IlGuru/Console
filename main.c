// gcc -o main.exe main.c stuff.c registry.c screen.c display.c -lcurses -lpthread 

#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include <windows.h>

#include "./registry.h"
#include "./screen.h"
#include "./stuff.h"

#define uchar unsigned char

// #define THREAD_SCREEN_REPAINT

uchar memory[ 4096 ];
uchar *mem_top;

void Init() {

	mem_top				= memory;
	
#ifdef _REGISTRY	
	p_registry			= (t_registry*) mem_top;
	mem_top				= (unsigned char *) p_registry + sizeof(t_registry);
	RegistryInit();
#endif	
#ifdef _SCREEN	
	p_screen            = (t_screen*) mem_top;
	mem_top				= (unsigned char *) p_screen + sizeof(t_screen);
	scrInit();
#endif	
	
}

void MemInfo() {
	printf("\n\rMemInfo:\n\r");
	printf( "BaseMem   %08X (%4d) Bytes\n\r", memory, sizeof(memory) );
	printf( "TopMem    %08X (%4d) Bytes\n\r", mem_top, mem_top-memory );
#ifdef _REGISTRY	
	printf( "RegMem    %08X (%4d) Bytes\n\r", p_registry, sizeof(t_registry) );
#endif	
#ifdef _SCREEN	
	printf( "ScreenMem %08X (%4d) Bytes\n\r", p_screen, sizeof(t_screen) );
#endif	
}

void DumpMem( int x_from, int x_to ) {
	int x,i;
	
	printf( "\n\rDump ( %08X ):\n\r", &memory[x_from] );
	i=0;
	for ( x=x_from; x<=x_to; x++ ) {
		if ( i>31 ) {
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
}

void DoScreenRepaint() {
	scrRepaint(0);
	
	// printf("\n\r");
	
	// MemInfo();
	
	#ifdef _REGISTRY	
	// DumpMem( (int) ((unsigned char*)p_registry-(unsigned char*)memory), (int) ((unsigned char*)p_registry-(unsigned char*)memory+sizeof(t_registry)-1) );
	#endif
	#ifdef _SCREEN	
	// DumpMem( (int) ((unsigned char*)p_screen-(unsigned char*)memory), (int) ((unsigned char*)p_screen-(unsigned char*)memory+sizeof(t_screen)-1) );
	#endif
}

#ifdef THREAD_SCREEN_REPAINT
pthread_t 	thScreenRepaint;

void *th_DoScreenRepaint( void *param ) {
	char *p_memory;
	p_memory = (char *) param;
	
	while ( 1 ) {
		DoScreenRepaint();
		sleepMs( 100 );
	}

	pthread_exit(NULL);
}
#endif
	
int main( int argc, char *argv[] ) {

	uchar c = '\0';
	
	Init();

#ifdef THREAD_SCREEN_REPAINT
	if ( pthread_create( &thScreenRepaint, NULL, th_DoScreenRepaint, (void *)memory ) ) {
		printf("error creating DoScreenRepaint thread.\n");
	}	
#endif
	
	while (1) {
		c = getch();
		scrWrite( c );
#ifndef THREAD_SCREEN_REPAINT
		DoScreenRepaint();
#endif
	}
	
	return 0;
}