#include "problem2.h"

/* * * * * * * * * * * * * * * * * * * * * * *
* Costume department initialization methods
* * * * * * * * * * * * * * * * * * * * *  * */



/* * * * * * * * * * * * * * * * * * * * * * *
*	Main()
* * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char **argv) {

	srand48(SEED_VAL);

	driver *d[20];

	unsigned int i;

	for(i = 0; i < 20; i++) {
		d[i] = (driver *) malloc(sizeof(driver));
		if(!d[i]) {
			printf("Cannot malloc() for driver!!!!!\n");
			return -1;
	    }

		d[i]->id = i;
		// random num from 0 to 3 for dirOrigin
		//create list with all the possibilities a driver can take (last num will be direction)
//		pthread_create(&d[i], NULL, Action, (void *) d[i]);

	}


	return 0;

}
