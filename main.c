#include <stdio.h>

unsigned char memory[4096];

//----------------------------------------------------
//	Screen

#define scr_xmax 15
#define scr_ymax 2

typedef struct {
	//	Buffer
	unsigned char 	Buffer[scr_xmax*scr_ymax];
	//	Registri
	short int	x_pos;
	short int	y_pos;
	short int	x_max;
	short int	y_max;
	short int	BufSize;
} t_scrMem;

t_scrMem *scrMem;

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
static void scr_Repaint(int amount) {
	int xg = 0;
	
	scr_clear();
		
	for (xg = 0 ; xg < amount ; xg++) {
		scr_write(scrMem->Buffer[xg]);		
	}

}

void DumpMem( int x_from, int x_to ) {
	int x,i;
	printf("\nDump:\n");
	printf( "BaseMem   %08X\n", memory );
	printf( "ScreenMem %08X\n", scrMem );
	
	i=0;
	for ( x=x_from; x<=x_to; x++ ) {
		if ( i>15 ) {
			printf("\n");
			i=0;
		}
		if ( i==0 ) {
			printf( "%04X: ", x );
		}
		printf( "%02X ", memory[x] );
		i++;
	}
	printf("\n");
}

int main() {

	scrMem              = (t_scrMem*) memory;
	scrMem_Init();
	
	scrMem->Buffer[0] = 'C';
	scrMem->Buffer[1] = 'i';
	scrMem->Buffer[2] = 'a';
	scrMem->Buffer[3] = 'o';
	scrMem->Buffer[4] = '.';
	
	scr_Repaint(scr_xmax*scr_ymax);
	DumpMem( 0, sizeof(*scrMem) );
	
	printf("\nRegistri:\n");
	printf("\n%d",scrMem->x_pos);
	printf("\n%d",scrMem->y_pos);
	printf("\n%d",scrMem->x_max);
	printf("\n%d",scrMem->y_max);
	printf("\n%d",scrMem->BufSize);
	
	return 0;

}