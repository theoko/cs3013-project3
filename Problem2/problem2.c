#include "problem2.h"

void initSem(void) {
	sem_init(&NE, 0, 1);
	sem_init(&NW, 0, 1);
	sem_init(&SW, 0, 1);
	sem_init(&SE, 0, 1);
	sem_init(&queue_lock, 0, 1);
	sem_init(&driver_num, 0, 0);
	sem_init(&drivers_waiting, 0, 0);
}

driver* getDriver(driver* head) {
	if (head->next == NULL) {
		printf("No more drivers.\n");
		return NULL;
	}
	sem_wait(&drivers_waiting);
	sem_wait(&queue_lock);
	driver* next_driver = head->next;
	head->next = next_driver->next;
	sem_post(&queue_lock);
	return next_driver;
}


void massDriver(void) {
    int random = rand();
    int randRoad = (random % 4) + 3;
    int randDirection = random % 3;
    driver* this_driver = malloc(sizeof(driver));
    this_driver->thread = pthread_self();
    this_driver->from = randRoad;
    this_driver->turn = randDirection;
    sem_post(&driver_num);
    sem_getvalue(&driver_num, &(this_driver->num));
    addDriver(&driverList, this_driver);
    switch(this_driver->from) {
        case NORTH:
            switch(this_driver->turn) {
		        case LEFT:
		            printf("driver %d coming from north and going left\n", this_driver->num);
		            break;
		        case STRAIGHT:
		            printf("driver %d coming from north and going straight\n", this_driver->num);
		            break;
		        case RIGHT:
		            printf("driver %d coming from north and going right\n", this_driver->num);
		            break;
		    }
			break;
        case EAST:
            switch(this_driver->turn) {
		        case LEFT:
		            printf("driver %d coming from east and going left\n", this_driver->num);
		            break;
		        case STRAIGHT:
		            printf("driver %d coming from east and going straight\n", this_driver->num);
		            break;
		        case RIGHT:
		            printf("driver %d coming from east and going right\n", this_driver->num);
		            break;
		    }
            break;
        case SOUTH:
            switch(this_driver->turn) {
		        case LEFT:
		            printf("driver %d coming from south and going left\n", this_driver->num);
		            break;
		        case STRAIGHT:
		            printf("driver %d coming from south and going straight\n", this_driver->num);
		            break;
		        case RIGHT:
		            printf("driver %d coming from south and going right\n", this_driver->num);
		            break;
		    }
            break;
        case WEST:
            switch(this_driver->turn) {
		        case LEFT:
		            printf("driver %d coming from west and going left\n", this_driver->num);
		            break;
		        case STRAIGHT:
		            printf("driver %d coming from west and going straight\n", this_driver->num);
		            break;
		        case RIGHT:
		            printf("driver %d coming from west and going right\n", this_driver->num);
		            break;
		    }
            break;
    }
}

void addDriver(driver* head, driver* add) {
	sem_wait(&queue_lock);
	driver* tail = head;
	while (tail->next != NULL) {
		tail = tail->next;
	}
	add->next = NULL;
	tail->next = add;
	sem_post(&drivers_waiting);
	sem_post(&queue_lock);
}

driver* get_driver(driver* head) {
	if (head->next == NULL) {
		printf("No more drivers! Start again.\n");
		return NULL;
	}
	sem_wait(&drivers_waiting);
	sem_wait(&queue_lock);
	driver* next_driver = head->next;
	head->next = next_driver->next;
	sem_post(&queue_lock);
	return next_driver;
}

void controlDriver(void) {
	while (1) {
		driver* next_driver = getDriver(&driverList);
		if (next_driver != NULL) {
			switch(next_driver->from) {
				case NORTH:
					switch(next_driver->turn) {
						case LEFT:
							sem_wait(&NW);
							printf("driver %d entered NW\n", next_driver->num);
							sleep(rand()%3);
							sem_wait(&SW);
							printf("driver %d entered SW\n", next_driver->num);
							sem_post(&NW);
							sem_wait(&SE);
							printf("driver %d entered SE\n", next_driver->num);
							sem_post(&SW);
							sleep(rand()%4);
							printf("driver %d exited!\n", next_driver->num);
							sem_post(&SE);
						break;
						case STRAIGHT:
							sem_wait(&NW);
							printf("driver %d entered NW\n", next_driver->num);
							sleep(rand()%4);
							sem_wait(&SW);
							printf("driver %d entered SW\n", next_driver->num);
							sem_post(&NW);
							printf("driver %d exited!\n", next_driver->num);
							sleep(rand()%1);
							sem_post(&SW);
						break;
						case RIGHT:
							sem_wait(&NW);
							printf("driver %d entered NW\n", next_driver->num);
							printf("driver %d exited!\n", next_driver->num);
							sem_post(&NW);
						break;
					}
				break;
				case EAST:
					switch(next_driver->turn) {
						case LEFT:
							sem_wait(&NE);
							printf("driver %d entered NE\n", next_driver->num);
							sem_wait(&NW);
							sleep(rand()%3);
							printf("driver %d entered NW\n", next_driver->num);
							sem_post(&NE);
							sem_wait(&SW);
							sleep(rand()%4);
							printf("driver %d entered SW\n", next_driver->num);
							sem_post(&NW);
							printf("driver %d exited!\n", next_driver->num);
							sem_post(&SW);
						break;
						case STRAIGHT:
							sem_wait(&NE);
							sleep(rand()%1);
							printf("driver %d entered NE\n", next_driver->num);
							sem_wait(&NW);
							printf("driver %d entered NW\n", next_driver->num);
							sem_post(&NE);
							sleep(rand()%4);
							printf("driver %d exited!\n", next_driver->num);
							sem_post(&NW);
						break;
						case RIGHT:
							sem_wait(&NE);
							sleep(rand()%1);
							printf("driver %d entered NE\n", next_driver->num);
							printf("driver %d exited!\n", next_driver->num);
							sem_post(&NE);
						break;
					}
				break;
				case SOUTH:
					switch(next_driver->turn) {
						case LEFT:
							sem_wait(&SE);
							printf("driver %d entered SE\n", next_driver->num);
							sleep(rand()%1);
							sem_wait(&NE);
							printf("driver %d entered NE\n", next_driver->num);
							sem_post(&SE);
							sem_wait(&NW);
							sleep(rand()%4);
							printf("driver %d entered NW\n", next_driver->num);
							sem_post(&NE);
							printf("driver %d exited!\n", next_driver->num);
							sem_post(&NW);
						break;
						case STRAIGHT:
							sem_wait(&SE);
							printf("driver %d entered SE\n", next_driver->num);
							sem_wait(&NE);
							sleep(rand()%1);
							printf("driver %d entered NE\n", next_driver->num);
							sem_post(&SE);
							sleep(rand()%3);
							printf("driver %d exited!\n", next_driver->num);
							sem_post(&NE);
						break;
						case RIGHT:
							sem_wait(&SE);
							sleep(rand()%3);
							printf("driver %d entered SE\n", next_driver->num);
							printf("driver %d exited!\n", next_driver->num);
							sem_post(&SE);
						break;
					}
				break;
				case WEST:
					switch(next_driver->turn) {
						case LEFT:
							sem_wait(&SW);
							printf("driver %d entered SW\n", next_driver->num);
							sem_wait(&SE);
							printf("driver %d entered SE\n", next_driver->num);
							sleep(rand()%3);
							sem_post(&SW);
							sem_wait(&NE);
							printf("driver %d entered NE\n", next_driver->num);
							sem_post(&SE);
							sleep(rand()%3);
							printf("driver %d exited!\n", next_driver->num);
							sem_post(&NE);
						break;
						case STRAIGHT:
							sem_wait(&SW);
							printf("driver %d entered SW\n", next_driver->num);
							sem_wait(&SE);
							sleep(rand()%3);
							printf("driver %d entered SE\n", next_driver->num);
							sem_post(&SW);
							printf("driver %d exited!\n", next_driver->num);
							sem_post(&SE);
						break;
						case RIGHT:
							sem_wait(&SW);
							sleep(rand()%3);
							printf("driver %d entered SW\n", next_driver->num);
							printf("driver %d exited!\n", next_driver->num);
							sem_post(&SW);
						break;
					}
				break;
			}
		}
		else {
			free(next_driver);
			pthread_exit(NULL);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * *
*	Main()
* * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv) {

	srand(time(NULL));

	driverList.next = NULL;

	initSem();

	pthread_t driver_threads[20];
	pthread_t control_threads[3]; // only three to avoid deadlock
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int i;
	while(1) {
		for(i = 0; i < 20; i++)
			pthread_create(&driver_threads[i], &attr, (void*)&massDriver, NULL);
		// start the control threads
		pthread_create(&control_threads[0], &attr, (void*)&controlDriver, NULL);
		pthread_create(&control_threads[1], &attr, (void*)&controlDriver, NULL);
		pthread_create(&control_threads[2], &attr, (void*)&controlDriver, NULL);
		pthread_join(control_threads[0], NULL);
		pthread_join(control_threads[1], NULL);
		pthread_join(control_threads[2], NULL);
	}

	return 0;
}

