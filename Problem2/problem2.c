#include "problem2.h"

/* * * * * * * * * * * * * * * * * * * * * * *
* Costume department initialization methods
* * * * * * * * * * * * * * * * * * * * *  * */



/* * * * * * * * * * * * * * * * * * * * * * *
*	Main()
* * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv) {
	srand(time(NULL));
	car_queue.next = NULL;
	sem_init(&road_nw, 0, 1);
	sem_init(&road_ne, 0, 1);
	sem_init(&road_sw, 0, 1);
	sem_init(&road_se, 0, 1);
	sem_init(&queue_lock, 0, 1);
	sem_init(&car_num, 0, 0);
	sem_init(&cars_waiting, 0, 0);
	pthread_t car_threads[NUM_CARS];
	pthread_t control_threads[3]; // only three to avoid deadlock
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int i;
	while(TRUE) {
		for(i = 0; i < NUM_CARS; i++)
			pthread_create(&car_threads[i], &attr, (void*)&masshole, NULL);
		// start the control threads
		pthread_create(&control_threads[0], &attr, (void*)&car_control, NULL);
		pthread_create(&control_threads[1], &attr, (void*)&car_control, NULL);
		pthread_create(&control_threads[2], &attr, (void*)&car_control, NULL);
		pthread_join(control_threads[0], NULL);
		pthread_join(control_threads[1], NULL);
		pthread_join(control_threads[2], NULL);
	}
	return 0;
}

void masshole(void) {
    int random = rand();
    int randRoad = (random % 4) + 3;
    int randDirection = random % 3;
    car* this_car = malloc(sizeof(car));
    this_car->thread = pthread_self();
    this_car->from = randRoad;
    this_car->turn = randDirection;
    sem_post(&car_num);
    sem_getvalue(&car_num, &(this_car->num));
    add_car(&car_queue, this_car);
    switch(this_car->from) {
        case NORTH:
            switch(this_car->turn) {
		        case LEFT:
		            printf("Car %d entering from north, going left...\n", this_car->num);
		            break;
		        case STRAIGHT:
		            printf("Car %d entering from north, going straight...\n", this_car->num);
		            break;
		        case RIGHT:
		            printf("Car %d entering from north, going right...\n", this_car->num);
		            break;
		    }
			break;
        case EAST:
            switch(this_car->turn) {
		        case LEFT:
		            printf("Car %d entering from east, going left...\n", this_car->num);
		            break;
		        case STRAIGHT:
		            printf("Car %d entering from east, going straight...\n", this_car->num);
		            break;
		        case RIGHT:
		            printf("Car %d entering from east, going right...\n", this_car->num);
		            break;
		    }
            break;
        case SOUTH:
            switch(this_car->turn) {
		        case LEFT:
		            printf("Car %d entering from south, going left...\n", this_car->num);
		            break;
		        case STRAIGHT:
		            printf("Car %d entering from south, going straight...\n", this_car->num);
		            break;
		        case RIGHT:
		            printf("Car %d entering from south, going right...\n", this_car->num);
		            break;
		    }
            break;
        case WEST:
            switch(this_car->turn) {
		        case LEFT:
		            printf("Car %d entering from west, going left...\n", this_car->num);
		            break;
		        case STRAIGHT:
		            printf("Car %d entering from west, going straight...\n", this_car->num);
		            break;
		        case RIGHT:
		            printf("Car %d entering from west, going right...\n", this_car->num);
		            break;
		    }
            break;
    }
}

void add_car(car* head, car* add) {
	sem_wait(&queue_lock);
	car* tail = head;
	while (tail->next != NULL) {
		tail = tail->next;
	}
	add->next = NULL;
	tail->next = add;
	sem_post(&cars_waiting);
	sem_post(&queue_lock);
}

car* get_car(car* head) {
	if (head->next == NULL) {
		printf("No more cars! Start again.\n");
		return NULL;
	}
	sem_wait(&cars_waiting);
	sem_wait(&queue_lock);
	car* next_car = head->next;
	head->next = next_car->next;
	sem_post(&queue_lock);
	return next_car;
}

void car_control(void) {
	while (TRUE) {
		car* next_car = get_car(&car_queue);
		if (next_car != NULL) {
			switch(next_car->from) {
				case NORTH:
					switch(next_car->turn) {
						case LEFT:
							sem_wait(&road_nw);
							printf("Car %d entered NW\n", next_car->num);
							sleep(rand()%3);
							sem_wait(&road_sw);
							printf("Car %d entered SW\n", next_car->num);
							sem_post(&road_nw);
							sem_wait(&road_se);
							printf("Car %d entered SE\n", next_car->num);
							sem_post(&road_sw);
							sleep(rand()%4);
							printf("Car %d exited\n", next_car->num);
							sem_post(&road_se);
						break;
						case STRAIGHT:
							sem_wait(&road_nw);
							printf("Car %d entered NW\n", next_car->num);
							sleep(rand()%4);
							sem_wait(&road_sw);
							printf("Car %d entered SW\n", next_car->num);
							sem_post(&road_nw);
							printf("Car %d exited\n", next_car->num);
							sleep(rand()%1);
							sem_post(&road_sw);
						break;
						case RIGHT:
							sem_wait(&road_nw);
							printf("Car %d entered NW\n", next_car->num);
							printf("Car %d exited\n", next_car->num);
							sem_post(&road_nw);
						break;
					}
				break;
				case EAST:
					switch(next_car->turn) {
						case LEFT:
							sem_wait(&road_ne);
							printf("Car %d entered NE\n", next_car->num);
							sem_wait(&road_nw);
							sleep(rand()%3);
							printf("Car %d entered NW\n", next_car->num);
							sem_post(&road_ne);
							sem_wait(&road_sw);
							sleep(rand()%4);
							printf("Car %d entered SW\n", next_car->num);
							sem_post(&road_nw);
							printf("Car %d exited\n", next_car->num);
							sem_post(&road_sw);
						break;
						case STRAIGHT:
							sem_wait(&road_ne);
							sleep(rand()%1);
							printf("Car %d entered NE\n", next_car->num);
							sem_wait(&road_nw);
							printf("Car %d entered NW\n", next_car->num);
							sem_post(&road_ne);
							sleep(rand()%4);
							printf("Car %d exited\n", next_car->num);
							sem_post(&road_nw);
						break;
						case RIGHT:
							sem_wait(&road_ne);
							sleep(rand()%1);
							printf("Car %d entered NE\n", next_car->num);
							printf("Car %d exited\n", next_car->num);
							sem_post(&road_ne);
						break;
					}
				break;
				case SOUTH:
					switch(next_car->turn) {
						case LEFT:
							sem_wait(&road_se);
							printf("Car %d entered SE\n", next_car->num);
							sleep(rand()%1);
							sem_wait(&road_ne);
							printf("Car %d entered NE\n", next_car->num);
							sem_post(&road_se);
							sem_wait(&road_nw);
							sleep(rand()%4);
							printf("Car %d entered NW\n", next_car->num);
							sem_post(&road_ne);
							printf("Car %d exited\n", next_car->num);
							sem_post(&road_nw);
						break;
						case STRAIGHT:
							sem_wait(&road_se);
							printf("Car %d entered SE\n", next_car->num);
							sem_wait(&road_ne);
							sleep(rand()%1);
							printf("Car %d entered NE\n", next_car->num);
							sem_post(&road_se);
							sleep(rand()%3);
							printf("Car %d exited\n", next_car->num);
							sem_post(&road_ne);
						break;
						case RIGHT:
							sem_wait(&road_se);
							sleep(rand()%3);
							printf("Car %d entered SE\n", next_car->num);
							printf("Car %d exited\n", next_car->num);
							sem_post(&road_se);
						break;
					}
				break;
				case WEST:
					switch(next_car->turn) {
						case LEFT:
							sem_wait(&road_sw);
							printf("Car %d entered SW\n", next_car->num);
							sem_wait(&road_se);
							printf("Car %d entered SE\n", next_car->num);
							sleep(rand()%3);
							sem_post(&road_sw);
							sem_wait(&road_ne);
							printf("Car %d entered NE\n", next_car->num);
							sem_post(&road_se);
							sleep(rand()%3);
							printf("Car %d exited\n", next_car->num);
							sem_post(&road_ne);
						break;
						case STRAIGHT:
							sem_wait(&road_sw);
							printf("Car %d entered SW\n", next_car->num);
							sem_wait(&road_se);
							sleep(rand()%3);
							printf("Car %d entered SE\n", next_car->num);
							sem_post(&road_sw);
							printf("Car %d exited\n", next_car->num);
							sem_post(&road_se);
						break;
						case RIGHT:
							sem_wait(&road_sw);
							sleep(rand()%3);
							printf("Car %d entered SW\n", next_car->num);
							printf("Car %d exited\n", next_car->num);
							sem_post(&road_sw);
						break;
					}
				break;
			}
		}
		else {
			free(next_car);
			pthread_exit(NULL);
		}
	}
}
