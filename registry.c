#include "./registry.h"

//----------------------------------------------------
//	Registry

#ifdef SYSTEM_TIME
void *DoSystemTime( void *param ) {
	t_registry *p_registry;
	p_registry = (t_registry *) param;

	while ( 1 ) {
		p_registry->SystemTime++;
		sleepMs( 1000 );
	}

	pthread_exit(NULL);
}
#endif

void RegistryInit() {
#ifdef SYSTEM_TIME
	pthread_t 	thSystemTime;
	
	p_registry->SystemTime	= 0;
	if ( pthread_create( &thSystemTime, NULL, DoSystemTime, (void *)p_registry ) ) {
		printf("error creating DoSystemTime thread.\n");
	}	
#endif	
}
