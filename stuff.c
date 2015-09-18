#include <stdio.h>
#include "./screen.h"

unsigned char memory[4096];

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