#include "./registry.h"

//----------------------------------------------------
//	Registry

void *DoHearthBeat( void *param ) {
	t_registry *p_registry;
	p_registry = (t_registry *) param;

	while ( 1 ) {
		p_registry->HearthBeat++;
		sleepMs( 1000 );
	}

	pthread_exit(NULL);
}

void RegistryInit() {
	pthread_t 	thHearthBeat;
	
	p_registry->HearthBeat	= 0;
	if ( pthread_create( &thHearthBeat, NULL, DoHearthBeat, (void *)p_registry ) ) {
		printf("error creating DoHearthBeat thread.\n");
	}	
	
}
