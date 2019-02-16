#ifndef PROBLEM2_H
#define PROBLEM2_H

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define TRUE 1
#define NUM_CARS 20
#define LEFT 0
#define STRAIGHT 1
#define RIGHT 2
#define NORTH 3
#define EAST 4
#define SOUTH 5
#define WEST 6

sem_t road_nw;
sem_t road_ne;
sem_t road_sw;
sem_t road_se;
sem_t car_num;
sem_t queue_lock;
sem_t cars_waiting;

struct car_struct{
	pthread_t thread;
	char from;
	char turn;
	int num;
	struct car_struct* next;
};
typedef struct car_struct car;

void masshole(void);
void add_car(car* head, car* add);
car* get_car(car* head);
void car_control(void);

// FIFO queue for all cars, regardless of direction
car car_queue;

#endif
