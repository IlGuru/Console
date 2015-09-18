#ifndef _REGISTRY

#define _REGISTRY

#include <stdio.h>
#include <pthread.h>
#include "./stuff.h"

// #define SYSTEM_TIME

typedef struct {
#ifdef SYSTEM_TIME
	long		SystemTime;
#endif	
} t_registry;

t_registry *p_registry;

void RegistryInit();

#endif
