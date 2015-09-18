#include "./stuff.h"

void sleepMs( long ms ) {
    struct timespec ts;
	if ( ms > 0 ) {
		ts.tv_sec  = ms/1000;
		ts.tv_nsec = ( ms*1000 ) % 1000000;
		
		nanosleep(&ts, NULL);
	}
}

