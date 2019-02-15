#ifndef PROBLEM1_H
#define PROBLEM1_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/time.h>
#include <math.h>
#include <limits.h>

#define SEED_VAL (100)

sem_t s[4];

typedef struct g_list
{
    void  *data; // any data
    struct g_list *next; // pointer to next element
} GList;

typedef struct driver {

	//list of quadrants driver will pass through
	GList *quadrants;
	int dirOrigin; // 0 for North, 1 South, 2 for West, 3 for East
	int id;
} driver;

#endif
