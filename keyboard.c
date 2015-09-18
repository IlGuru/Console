#include "./keyboard.h"

//----------------------------------------------------
//	Keyboard

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
	if ( !( TSTBIT( p_keyboard->status , KB_BUFFER_FULL ) ) ) {
		p_keyboard->Buffer[p_keyboard->BufPos] = getch();
	}
	kbIncBufPos();
}

uchar kbGettCh() {
	uchar c = '\0';
	
	if ( !( TSTBIT( p_keyboard->status , KB_BUFFER_EMPTY ) ) ) {
		kbDecBufPos();
		c = p_keyboard->Buffer[p_keyboard->BufPos];
		p_keyboard->Buffer[p_keyboard->BufPos] = '\0';
	}
	return c;
}

void kbInit() {
	kbClear();

	cbreak(); // curses call to set no waiting for Enter key
	noecho(); // curses call to set no echoing
}
