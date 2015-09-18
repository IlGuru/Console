#ifndef _REGISTRY

#define _REGISTRY

#define KeyBoard_BufSize 8

typedef struct {
	char Buffer[KeyBoard_BufSize];
	char BufSize;
	char BufSize_Max;
} t_KeyBoard;

typedef struct {
	long		HearthBeat;
	t_KeyBoard	KeyBoard;
} t_regMem;

t_regMem *regMem;

void regMem_Init();

#endif
