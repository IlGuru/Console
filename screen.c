#include <stdio.h>
#include "./screen.h"

//----------------------------------------------------
//	Screen

//----------------------------------------------------

void scrCheckPosFlag() {
	if ( p_screen->x_pos <= SCR_POSX_MIN ) {
		SETBIT(p_screen->status,f_srcFirstColumn);
	} else {
		CLRBIT(p_screen->status,f_srcFirstColumn);
	}
	if ( p_screen->x_pos >= SCR_POSX_MAX ) {
		SETBIT(p_screen->status,f_srcLastColumn);
	} else {
		CLRBIT(p_screen->status,f_srcLastColumn);
	}
	if ( p_screen->y_pos <= SCR_POSY_MIN ) {
		SETBIT(p_screen->status,f_srcFirstRow);
	} else {
		CLRBIT(p_screen->status,f_srcFirstRow);
	}
	if ( p_screen->y_pos >= SCR_POSY_MAX ) {
		SETBIT(p_screen->status,f_srcLastRow);
	} else {
		CLRBIT(p_screen->status,f_srcLastRow);
	}
}

//----------------------------------------------------

void scrGoHomeX() {
	p_screen->x_pos = SCR_POSX_MIN;
	scrCheckPosFlag();
}

void scrGoHomeY() {
	p_screen->y_pos = SCR_POSY_MIN;
	scrCheckPosFlag();
}

void scrGoLastX() {
	p_screen->x_pos = SCR_POSX_MAX;
	scrCheckPosFlag();
}

void scrGoLastY() {
	p_screen->y_pos = SCR_POSY_MAX;
	scrCheckPosFlag();
}

void scrGoEndX() {
	p_screen->x_pos = SCR_XMAX_SIZE;
	scrCheckPosFlag();
}

void scrGoEndY() {
	p_screen->y_pos = SCR_YMAX_SIZE;
	scrCheckPosFlag();
}

void scrCursorHome() {
	p_screen->x_pos = SCR_POSX_MIN;
	p_screen->y_pos = SCR_POSY_MIN;
	scrCheckPosFlag();
}

void scrIncY() {

	p_screen->y_pos++;

	if ( p_screen->y_pos > SCR_POSY_MAX ) {
		scrGoLastY(); 
	}

	scrCheckPosFlag();

}

void scrIncX() {

	p_screen->x_pos++;

	if ( p_screen->x_pos > SCR_POSX_MAX ) {
		if ( p_screen->y_pos < SCR_POSY_MAX ) {
			scrGoHomeX();
			scrIncY();
		} else {
			scrGoEndX();
		}
	}		

	scrCheckPosFlag();

}

void scrDecY() {
	p_screen->y_pos--;
	if ( p_screen->y_pos < SCR_POSY_MIN ) {
		scrGoHomeY();
	}

	scrCheckPosFlag();

}

void scrDecX() {
	p_screen->x_pos--;
	if ( p_screen->x_pos < SCR_POSX_MIN ) {
		if ( p_screen->y_pos > SCR_POSY_MIN ) {
			scrGoLastX();
			scrDecY();
		} else {
			scrGoHomeX();
		}
	}

	scrCheckPosFlag();

}

//----------------------------------------------------

void scrClearMemBuf() {
	int xg = 0;
	for (xg = 0 ; xg < SCR_XMAX_SIZE*SCR_YMAX_SIZE ; xg++) {
		p_screen->Buffer[xg] = ' ';
	}
}

//----------------------------------------------------

void scrInit() {

	dspInit();

	p_screen->x_max		= SCR_XMAX_SIZE;
	p_screen->y_max		= SCR_YMAX_SIZE;
	CLRBIT(p_screen->status,f_srcContentChanged);
	
	scrClearMemBuf();

	scrRepaint();
	
}

//----------------------------------------------------

void scrBackSpace() {
	scrDecX();
	p_screen->Buffer[ p_screen->y_pos*SCR_XMAX_SIZE + p_screen->x_pos ] = ' ';
}

void scrReturn() {
	curscoord x,y;
	if ( p_screen->y_pos < SCR_POSY_MAX) {
		scrIncY();
	} else {
		for ( y=0; y<SCR_POSY_MAX; y++ ) {
			for ( x=0; x<SCR_XMAX_SIZE; x++ ) {
				p_screen->Buffer[ (y)*SCR_XMAX_SIZE + x ] = p_screen->Buffer[ (y+1)*SCR_XMAX_SIZE + x ];
			}
		}	
		for ( x=0; x<SCR_XMAX_SIZE; x++ ) {
			p_screen->Buffer[ (y)*SCR_XMAX_SIZE + x ] = ' ';
		}
	}
	scrGoHomeX();
}

//------------------------------------------------------------------------------------------------------

void scrWrite( unsigned char c ) {
	
	unsigned char d,e;
	
	switch ( c ) {
		case 8:
			scrBackSpace();
			break;
		case 127:
			scrBackSpace();
			break;
		case 10:
			scrReturn();
			scrRepaint();
			break;
		case 13:
			break;
		case 27:
			// d = getch();
			// switch ( d ) {
			kbPutCh();
			switch ( kbGettCh() ) {
				case 91:
					// e = getch();
					// switch ( e ) {
					kbPutCh();
					switch ( kbGettCh() ) {
						case 65:		//	KEY_UP
							scrDecY();
							break;
						case 66:		//	KEY_DOWN
							scrIncY();
							break;
						case 67:		//	KEY_RIGHT
							scrIncX();
							break;
						case 68:		//	KEY_LEFT
							scrDecX();
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			break;
		default:
			if ( p_screen->x_pos < SCR_XMAX_SIZE ) {
				p_screen->Buffer[ p_screen->y_pos*SCR_XMAX_SIZE + p_screen->x_pos ] = c;
				scrIncX();
			}
	}

	SETBIT(p_screen->status,f_srcContentChanged);
}

unsigned char scrRead() {
	return ( p_screen->Buffer[ p_screen->y_pos*SCR_XMAX_SIZE + p_screen->x_pos ] );
}

//----------------------------------------------------

void scrRepaint() {

	curscoord xs, xe, ys, ye, x, y;
	
	if ( TSTBIT(p_screen->status,f_srcContentChanged) ) {
	
		p_screen->x  = ( p_screen->x_pos<SCR_POSX_MAX ? p_screen->x_pos : SCR_POSX_MAX );
		p_screen->y  = ( p_screen->y_pos<SCR_POSY_MAX ? p_screen->y_pos : SCR_POSY_MAX );
		
		p_screen->xs = ( p_screen->x < dspXmax() ? 0 : p_screen->x - dspXmax() + 1 );
		p_screen->xe = p_screen->xs + dspXmax() - 1;
		p_screen->ys = ( p_screen->y < dspYmax() ? 0 : p_screen->y - dspYmax() + 1 );
		p_screen->ye = p_screen->ys + dspYmax() - 1;

		dspCursorHome();
		for ( y = p_screen->ys; y <= p_screen->ye; y++ ) {
			for ( x = p_screen->xs; x <= p_screen->xe; x++ ) {
				if ( x-p_screen->xs < SCR_XMAX_SIZE && y-p_screen->ys < SCR_YMAX_SIZE ) {
					dspWrite( p_screen->Buffer[ y*SCR_XMAX_SIZE + x ] );
				} else {
					dspWrite( '~' );
				}
			}
		}
		
		CLRBIT(p_screen->status,f_srcContentChanged);
		
	}
	
}
