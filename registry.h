#ifndef _REGISTRY

#define _REGISTRY

typedef struct {
	long		HearthBeat;
} t_regMem;

t_regMem *regMem;

void regMem_Init();

#endif
