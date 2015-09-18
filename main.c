#include <stdio.h>

#define UINT_SIZE sizeof(unsigned int)

unsigned char memory[4096];

//----------------------------------------------------
//	Screen

#define scr_xmax 20
#define scr_ymax 10

unsigned char *screenMem;

unsigned int scr_x = 0;
unsigned int scr_y = 0;

void screenBuf_clear() {
	int xg = 0;
	for (xg = 0 ; xg < scr_xmax*scr_ymax ; xg++) {
		screenMem[xg] = '\0';
	}
}

//----------------------------------------------------

void clrscr() {
	const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
	printf( CLEAR_SCREE_ANSI );
}

//----------------------------------------------------
void scr_clear() {
	scr_x = 0;
	scr_y = 0;

	clrscr();
}

void scr_write( char c ) {

	if ( scr_x >= scr_xmax ) {
		scr_x = 0;
		scr_y++;
		putchar('\n');
		putchar('\r');
	}		
	if ( scr_y >= scr_ymax ) {
		return;
	}		

	if ( c == '\0' ) c = ' ';
	if ( c == '\n' ) scr_y++;
	if ( c == '\r' ) scr_x = 0;

	scr_x++;
	putchar( c );
}

//----------------------------------------------------
static void doFrame(int amount) {
	int xg = 0;
	
	scr_clear();
		
	for (xg = 0 ; xg < amount ; xg++) {
		scr_write(screenMem[xg]);
	}

}

int main() {

	screenMem 			= memory;
	
	screenBuf_clear();
	
	screenMem[0] = 'C';
	screenMem[1] = 'i';
	screenMem[2] = 'a';
	screenMem[3] = 'o';
	screenMem[4] = '.';
	
	doFrame(scr_xmax*(scr_ymax+5));
	printf("\n%d",scr_x);
	printf("\n%d",scr_y);
	
	return 0;

}