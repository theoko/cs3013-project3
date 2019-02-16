#ifndef PROBLEM2_H
#define PROBLEM2_H

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define LEFT 0
#define STRAIGHT 1
#define RIGHT 2
#define NORTH 3
#define EAST 4
#define SOUTH 5
#define WEST 6

sem_t NW;
sem_t NE;
sem_t SW;
sem_t SE;
sem_t driver_num;
sem_t queue_lock;
sem_t drivers_waiting;

typedef struct driver{
	pthread_t thread;
	char from;
	char turn;
	int num;
	struct driver* next;
} driver;

void initSem(void);
void massDriver(void);
void addDriver(driver* head, driver* add);
driver* getDriver(driver* head);
void controlDriver(void);

// FIFO queue for all cars, regardless of direction
driver driverList;

#endif
