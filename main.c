#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include "./screen.h"
#include "./registry.h"
#include "./keyboard.h"

//	gcc -o main.exe main_003.c registry.c display.c screen.c keyboard.c -lcurses -lpthread

unsigned char memory[4096];
unsigned char *mem_top;

#define NUM_THREADS     5

void MemInfo() {
	printf("\nMemInfo:\n");
	printf( "BaseMem   %08X (%4d) Bytes\n", memory, sizeof(memory) );
	printf( "TopMem    %08X (%4d) Bytes\n", mem_top, mem_top-memory );
#ifdef _REGISTRY	
	printf( "RegMem    %08X (%4d) Bytes\n", regMem, sizeof(t_regMem) );
#endif	
#ifdef _KEYBOARD	
	printf( "KbdMem    %08X (%4d) Bytes\n", kbdMem, sizeof(t_kbdMem) );
#endif	
#ifdef _SCREEN	
	printf( "ScreenMem %08X (%4d) Bytes\n", scrMem, sizeof(t_scrMem) );
#endif	
}

void DumpMem( int x_from, int x_to ) {
	int x,i;
	
	printf( "\nDump ( %08X ):\n", &memory[x_from] );
	i=0;
	for ( x=x_from; x<=x_to; x++ ) {
		if ( i>31 ) {
			printf("\n");
			i=0;
		}
		if ( i==0 ) {
			printf( "%04X : ", x );
		}
		printf( "%02X ", memory[x] );
		i++;
	}
	printf("\n");
}

void Init() {

	mem_top				= memory;
	
#ifdef _REGISTRY	
	regMem				= (t_regMem*) mem_top;
	mem_top				= (unsigned char *) regMem + sizeof(t_regMem);
	regMem_Init();
#endif	
#ifdef _KEYBOARD	
	kbdMem				= (t_kbdMem*) mem_top;
	mem_top				= (unsigned char *) kbdMem + sizeof(t_kbdMem);
	KeyBoard_Init();
#endif	
#ifdef _SCREEN	
	scrMem              = (t_scrMem*) mem_top;
	mem_top				= (unsigned char *) scrMem + sizeof(t_scrMem);
	scrInit();
#endif	
	
}

// void sleep( time_t delay ) {
	// time_t timer0, timer1;
	
	// time( &timer0 );
	// do {
		// time( &timer1 );
	// } while ( ( timer1-timer0 ) < delay );
// }
void sleep( long j ) {
	long i,l;
	for (l=0; l<j; l++)
		for (i=0; i<j; i++)
			;
}

int main() {

	pthread_t threads[NUM_THREADS];
	int rc[NUM_THREADS];
	long t[NUM_THREADS];

	unsigned char c = ' ';
	int  i = 0;
	
	Init();

	t[0]  = 42;
	rc[0] = pthread_create( &threads[t[0]], NULL, KeyBoard_Scan, (void *)t[0] );
	printf( "rc[0] %08X rc[0] %08X\n", t[0], rc[0] );
	while ( 1 ) {
		// DumpMem( (int) ((unsigned char*)kbdMem-(unsigned char*)memory), (int) ((unsigned char*)kbdMem-(unsigned char*)memory+sizeof(t_kbdMem)) );
		sleep(10000);
	}	
#ifdef _SCREEN	
	scrClear();
	
	for ( i=0; i<SCR_XMAX_SIZE*SCR_YMAX_SIZE; i++ ) {
		if ( c > 'z' )
			c = ' ';
		scrWrite( c );
		c++;
	}

	scrMem->x_pos=0;
	scrMem->y_pos=0;
	scrRepaint();

	// scrMem->y_pos=0;
	// for ( scrMem->x_pos=0; scrMem->x_pos < scrMem->x_max; scrMem->x_pos++ ) {
		// sleep(10000);
		// scrRepaint();
	// }
	// scrMem->x_pos=scrMem->x_max-1;
	// for ( scrMem->y_pos=0; scrMem->y_pos < scrMem->y_max; scrMem->y_pos++ ) {
		// sleep(10000);
		// scrRepaint();
	// }
	// scrMem->y_pos=scrMem->y_max-1;
	// for ( scrMem->x_pos=scrMem->x_max-1; scrMem->x_pos > 0; scrMem->x_pos-- ) {
		// sleep(10000);
		// scrRepaint();
	// }
	// scrMem->x_pos=0;
	// for ( scrMem->y_pos=scrMem->y_max-1; scrMem->y_pos > 0; scrMem->y_pos-- ) {
		// sleep(10000);
		// scrRepaint();
	// } 
#endif	

	MemInfo();
	
// #ifdef _REGISTRY	
	// DumpMem( (int) ((unsigned char*)regMem-(unsigned char*)memory), (int) ((unsigned char*)regMem-(unsigned char*)memory+sizeof(t_regMem)) );
// #endif	
#ifdef _KEYBOARD	
	DumpMem( (int) ((unsigned char*)kbdMem-(unsigned char*)memory), (int) ((unsigned char*)kbdMem-(unsigned char*)memory+sizeof(t_kbdMem)) );
#endif	
// #ifdef _SCREEN	
	// DumpMem( (int) ((unsigned char*)scrMem-(unsigned char*)memory), (int) ((unsigned char*)scrMem-(unsigned char*)memory+sizeof(t_scrMem)) );
// #endif	
	
	pthread_exit( NULL );
	
	return 0;

}