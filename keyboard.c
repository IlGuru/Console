#include "./keyboard.h"

//----------------------------------------------------
//	Keyboard

//----------------------------------------------------

void kbCheckStatus() {
	if ( p_keyboard->BufPos <= KB_BUFFER_MIN_POS ) {
		SETBIT( p_keyboard->status , KB_BUFFER_EMPTY );
	} else {
		CLRBIT( p_keyboard->status , KB_BUFFER_EMPTY );
	}
	if ( p_keyboard->BufPos >= KB_BUFFER_SIZE ) {
		SETBIT( p_keyboard->status , KB_BUFFER_FULL );
	} else {
		CLRBIT( p_keyboard->status , KB_BUFFER_FULL );
	}
}

void kbIncBufPos() {
	p_keyboard->BufPos++;
	
	if ( p_keyboard->BufPos >= KB_BUFFER_SIZE ) 
		p_keyboard->BufPos = KB_BUFFER_SIZE;
		
	kbCheckStatus();
}

void kbDecBufPos() {
	p_keyboard->BufPos--;

	if ( p_keyboard->BufPos <= KB_BUFFER_MIN_POS ) 
		p_keyboard->BufPos = KB_BUFFER_MIN_POS;

	kbCheckStatus();
}

void kbClear() {
	for ( p_keyboard->BufPos = KB_BUFFER_MIN_POS; p_keyboard->BufPos < KB_BUFFER_SIZE; p_keyboard->BufPos++ )
		p_keyboard->Buffer[p_keyboard->BufPos] = '\0';
	
	p_keyboard->BufPos = KB_BUFFER_MIN_POS;
	
	kbCheckStatus();
}

void kbPutCh() {

	while ( TSTBIT( p_keyboard->status , KB_GETCH ) ) {
		;
	}
	
	if ( TSTBIT( p_keyboard->status , KB_PUTCH ) == 0 ) {
		SETBIT( p_keyboard->status , KB_PUTCH );
		if ( !( TSTBIT( p_keyboard->status , KB_BUFFER_FULL ) ) ) {
			p_keyboard->Buffer[p_keyboard->BufPos] = getch();
// printf("\n\r kbPutCh: BufPos %d c %02X", p_keyboard->BufPos, p_keyboard->Buffer[p_keyboard->BufPos] );
			kbIncBufPos();
		}
		CLRBIT( p_keyboard->status , KB_PUTCH );
	}
}

uchar kbGetCh() {

	uchar c = '\0';
	uchar i;
	
	while ( TSTBIT( p_keyboard->status , KB_PUTCH ) ) {
		;
	}
	
	SETBIT( p_keyboard->status , KB_GETCH );
	if ( !( TSTBIT( p_keyboard->status , KB_BUFFER_EMPTY ) ) ) {
		c = p_keyboard->Buffer[ KB_BUFFER_MIN_POS ];
		for ( i=KB_BUFFER_MIN_POS ; i < p_keyboard->BufPos ; i++ ) {
			p_keyboard->Buffer[ i ] = p_keyboard->Buffer[ i + 1 ];
// printf("\n\r kbGetCh: i %d BufPos %d c %02X", i, p_keyboard->BufPos, p_keyboard->Buffer[i] );
		}
		p_keyboard->Buffer[ p_keyboard->BufPos ] = '\0';
		kbDecBufPos();
	}
	CLRBIT( p_keyboard->status , KB_GETCH );
	return c;
}

//----------------------------------------------------

#ifdef THREAD_KEYBOARD	

void *th_DoReadKB( void *param ) {
	FNINPUT fCallBack;
	fCallBack = (FNINPUT)(param);
	
	while ( 1 ) {
		if ( fCallBack != NULL ) {
			fCallBack();
		} else {
			kbPutCh();
		}
		#if KB_MS_REFRESH > 0
		sleepMs( KB_MS_REFRESH );
		#endif
	}
	pthread_exit(NULL);
}	

void kbCreateThread( FNINPUT fCallBack ) {
	if ( pthread_create( &thReadKB, NULL, th_DoReadKB, fCallBack ) ) {
		printf("error creating th_DoReadKB thread.\n");
	}	
}
#endif

void kbInit() {

	kbClear();

#ifdef THREAD_KEYBOARD	
	//kbCreateThread( kbPutCh );
	kbCreateThread( NULL );
#endif

	cbreak(); // curses call to set no waiting for Enter key
	noecho(); // curses call to set no echoing

}

