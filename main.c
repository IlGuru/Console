#include <stdio.h>

#define UINT_SIZE sizeof(unsigned int)

unsigned char memory[4096];

//----------------------------------------------------
//	Screen

#define scr_xmax 20
#define scr_ymax 10

unsigned char *screenMem;
unsigned char *screenMem_Buf;
unsigned char *screenMem_Reg;
unsigned int  *screenMem_Reg_scr_x;
unsigned int  *screenMem_Reg_scr_y;

void screenBuf_clear() {
	int xg = 0;
	for (xg = 0 ; xg < scr_xmax*scr_ymax ; xg++) {
		screenMem_Buf[xg] = '\0';
	}
}

//----------------------------------------------------

void clrscr() {
	const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
	printf( CLEAR_SCREE_ANSI );
}

//----------------------------------------------------
void scr_clear() {
	screenMem_Reg_scr_x = 0;
	screenMem_Reg_scr_y = 0;
	clrscr();
}

void scr_write( char c ) {

	if ( *screenMem_Reg_scr_x >= scr_xmax ) {
		screenMem_Reg_scr_x = 0;
		screenMem_Reg_scr_y++;
		putchar('\n');
		putchar('\r');
	}		
	if ( *screenMem_Reg_scr_y >= scr_ymax ) {
		return;
	}		

	if ( c == '\0' ) c = ' ';
	if ( c == '\n' ) screenMem_Reg_scr_y++;
	if ( c == '\r' ) screenMem_Reg_scr_x = 0;

	screenMem_Reg_scr_x++;
	putchar( c );
}

//----------------------------------------------------
static void doFrame(int amount) {
	int xg = 0;
	
	scr_clear();
		
	for (xg = 0 ; xg < amount ; xg++) {
		scr_write(screenMem_Buf[xg]);
	}

}

int main() {


	screenMem 			= memory;
	screenMem_Buf		= screenMem;
	screenMem_Reg		= screenMem+(scr_xmax*scr_ymax);
	screenMem_Reg_scr_x = (int*) screenMem_Reg;
	screenMem_Reg_scr_x = (int*) screenMem_Reg + UINT_SIZE;
	
	screenBuf_clear();
	
	screenMem_Buf[0] = 'C';
	screenMem_Buf[1] = 'i';
	screenMem_Buf[2] = 'a';
	screenMem_Buf[3] = 'o';
	screenMem_Buf[4] = '.';
	
	scr_clear();
//	doFrame(scr_xmax*(scr_ymax+1));
	printf("\n%d",screenMem_Reg_scr_x);
	printf("\n%d",screenMem_Reg_scr_y);
	
	return 0;

}