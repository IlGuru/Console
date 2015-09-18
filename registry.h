#ifndef _REGISTRY

#define _REGISTRY

#include <stdio.h>
#include <pthread.h>
#include "./stuff.h"

typedef struct {
	long		HearthBeat;
} t_registry;

t_registry *p_registry;

void RegistryInit();

#endif
