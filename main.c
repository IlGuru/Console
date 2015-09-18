#include <stdio.h>
#include "./screen.h"
#include "./registry.h"

unsigned char memory[4096];

void MemInfo() {
	printf("\nMemInfo:\n");
	printf( "BaseMem   %08X\n", memory );
	printf( "RegMem    %08X\n", regMem );
	printf( "ScreenMem %08X\n", scrMem );
}

void DumpMem( int x_from, int x_to ) {
	int x,i;
	
	printf("\nDump:\n");
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

void Init() {
	regMem				= (t_regMem*) memory;
	regMem_Init();
	
	scrMem              = (t_scrMem*) regMem + sizeof(t_regMem);
	scrMem_Init();
}

int main() {

	Init();
	
	scrMem->Buffer[0] = 'C';
	scrMem->Buffer[1] = 'i';
	scrMem->Buffer[2] = 'a';
	scrMem->Buffer[3] = 'o';
	scrMem->Buffer[4] = '.';
	
	scr_Repaint(scr_xmax*scr_ymax);
	MemInfo();
	DumpMem( (void*)regMem-(void*)memory, (void*)regMem-(void*)memory+sizeof(t_regMem) );
	DumpMem( (void*)scrMem-(void*)memory, (void*)scrMem-(void*)memory+sizeof(t_scrMem) );
	
	printf("\nRegistri:\n");
	printf("\n%d",scrMem->x_pos);
	printf("\n%d",scrMem->y_pos);
	printf("\n%d",scrMem->x_max);
	printf("\n%d",scrMem->y_max);
	printf("\n%d",scrMem->BufSize);
	
	return 0;

}