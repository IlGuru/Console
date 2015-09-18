#include "./keyboard.h"

void KeyBoard_ClearBuf() {
	for (kbdMem->BufPos = sizeof( kbdMem->Buffer ); kbdMem->BufPos >= 0; kbdMem->BufPos--) {
		kbdMem->Buffer[ kbdMem->BufPos ] = 255;
	}
}
void KeyBoard_Init() {
	KeyBoard_ClearBuf();

	kbdMem->BufPos = 0;
	kbdMem->Status = 0;
}

void KeyBoard_BufPosInc() {
	kbdMem->BufPos++;
	if ( kbdMem->BufPos > sizeof( kbdMem->Buffer ) ) kbdMem->BufPos = sizeof( kbdMem->Buffer );
}

void KeyBoard_BufPosDec() {
	kbdMem->BufPos--;
	if ( kbdMem->BufPos < 0 ) kbdMem->BufPos = 0;
}

void KeyBoard_CheckFlag() {
	if ( kbdMem->BufPos == 0 ) {
		kbdMem->Status = kbdMem->Status BufferEmpty_fSet;
	} else {
		kbdMem->Status = kbdMem->Status BufferEmpty_fClr;
	}
	if ( kbdMem->BufPos == sizeof( kbdMem->Buffer ) ) {
		kbdMem->Status = kbdMem->Status BufferOverflow_fSet;
	} else {
		kbdMem->Status = kbdMem->Status BufferOverflow_fClr;
	}
}

void *KeyBoard_Scan( void *threadid ) {
	
	unsigned char d;
	
	long tid;
	tid = (long)threadid;
	printf("Hello World! It's me, thread #%ld!\n", tid);
	printf("%08X\n", kbdMem);

    while ( 1 ) {
		// printf("%02X\n", kbdMem->Status);
		// if ( !( kbdMem->Status BufferOverflow_fChk ) ) { 
			d = getch();
			kbdMem->Buffer[ kbdMem->BufPos ] = d;
			kbdMem->BufPos++;
		// }
		KeyBoard_CheckFlag();
	}
	
	pthread_exit( NULL );
	
}