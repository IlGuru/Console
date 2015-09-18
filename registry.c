#include "./registry.h"

//----------------------------------------------------
//	Registry

void KeyBoard_ClearBuf() {
	int xg = 0;
	for (xg = 0 ; xg < KeyBoard_BufSize ; xg++) {
		regMem->KeyBoard.Buffer[xg] = 255;
	}
}
void KeyBoard_Init() {
	KeyBoard_ClearBuf();

	regMem->KeyBoard.BufSize = 0;
	regMem->KeyBoard.BufSize_Max = KeyBoard_BufSize;
}

void regMem_Init() {
	regMem->HearthBeat	= 0;
	
	KeyBoard_Init();
}
