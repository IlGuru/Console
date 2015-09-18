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
	if ( p_screen->y_pos == SCR_POSY_MIN ) {
		scrGoHomeY();
	} else {
		p_screen->y_pos--;
	}
	
	scrCheckPosFlag();

}

void scrDecX() {
	if ( p_screen->x_pos == SCR_POSX_MIN ) {
		if ( p_screen->y_pos > SCR_POSY_MIN ) {
			scrGoLastX();
			scrDecY();
		} else {
			scrGoHomeX();
		}
	} else {
		p_screen->x_pos--;
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

#ifdef THREAD_SCREEN_REPAINT
void *th_DoScreenRepaint( void *param ) {
	FNINPUT fCallBack;
	fCallBack = (FNINPUT)(param);
	
	while ( 1 ) {
		if ( fCallBack != NULL ) {
			fCallBack();
		} else {
			scrRepaint();
		}			
		#if SCR_MS_REFRESH != 0
		sleepMs( SCR_MS_REFRESH );
		#endif
	}

	pthread_exit( NULL );
}

void scrCreateThread( FNINPUT fCallBack ) {
	if ( pthread_create( &thScreenRepaint, NULL, th_DoScreenRepaint, fCallBack ) ) {
		printf("error creating th_DoScreenRepaint thread.\n");
	}	
}

#endif

void scrInit( FNINPUT fScreenRepaint ) {
	
	dspInit();

	p_screen->x_max		= SCR_XMAX_SIZE;
	p_screen->y_max		= SCR_YMAX_SIZE;
	
	SETBIT(p_screen->status,f_srcContentChanged);
	
	scrClearMemBuf();

#ifdef THREAD_SCREEN_REPAINT
	scrCreateThread( fScreenRepaint );
#else
	if ( fScreenRepaint != NULL ) {
		fScreenRepaint();
	} else {
		scrRepaint();
	}
#endif
	
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
			#ifdef _KEYBOARD
				#ifndef THREAD_KEYBOARD	
					kbPutCh();
				#endif
				switch ( kbGetCh() ) {
			#else
				d = getch();
				switch ( d ) {
			#endif
				case 91:
				#ifdef _KEYBOARD
					#ifndef THREAD_KEYBOARD	
						kbPutCh();
					#endif
					switch ( kbGetCh() ) {
				#else
					e = getch();
					switch ( e ) {
				#endif
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

	curscoord x, y, xs, xe, ys, ye;
	
	if ( TSTBIT(p_screen->status,f_srcRepainting) )
		return;
		
	SETBIT(p_screen->status,f_srcRepainting);
	
	if ( TSTBIT(p_screen->status,f_srcContentChanged) ) {
	
		x  = ( p_screen->x_pos<SCR_POSX_MAX ? p_screen->x_pos : SCR_POSX_MAX );
		y  = ( p_screen->y_pos<SCR_POSY_MAX ? p_screen->y_pos : SCR_POSY_MAX );
		
		xs = ( x < dspXmax( p_display ) ? 0 : x - dspXmax( p_display ) + 1 );
		xe = xs + dspXmax( p_display ) - 1;
		ys = ( y < dspYmax( p_display ) ? 0 : y - dspYmax( p_display ) + 1 );
		ye = ys + dspYmax( p_display ) - 1;

		dspCursorHome( p_display );
		for ( y = ys; y <= ye; y++ ) {
			for ( x = xs; x <= xe; x++ ) {
				if ( x-xs < SCR_XMAX_SIZE && y-ys < SCR_YMAX_SIZE ) {
					dspWrite( p_display, p_screen->Buffer[ y*SCR_XMAX_SIZE + x ] );
				} else {
					dspWrite( p_display, '~' );
				}
			}
		}
		#ifndef THREAD_DISPLAY_REPAINT
		dspRefresh( p_display );
		#endif
	
		CLRBIT(p_screen->status,f_srcContentChanged);
		
	}
	
	CLRBIT(p_screen->status,f_srcRepainting);
	
}
