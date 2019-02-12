#ifndef PROBLEM1_H
#define PROBLEM1_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

// General linked list
typedef struct g_list
{
    void  *data; // any data
    struct g_list *next; // pointer to next element
} GList;

// Queue
typedef struct g_queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
} GQueue;

typedef enum fighter_type {
  ninja, pirate
} fighter;

// Ninja:
//  - owes
//  - number of visits
//  - visit time
//  - time waiting
typedef struct ninja_node {
  int owes; // Amount of gold owed
  int numOfVisits; // Number of visits
  GList* visitTime; // List which will keep track of time spent on each visit
  GList* waitTime; // List which will keep track of time spent waiting
  fighter type;
} ninja;

// Pirate:
//  - owes
//  - number of visits
//  - time waiting
typedef struct pirate_node {
  int owes; // Amount of gold owed
  int numOfVisits;
  GList* visitTime; // List which will keep track of time spent on each visit
  GList* waitTime; // List which will keep track of time spent waiting
  fighter type;
} pirate;

// Team:
//  - time busy
//  - time free
//  - number of customers served
//  - amount of gold
typedef struct team_node {
  GList* busy; // List to keep track of time that a team is busy
  GList* free; // List to keep track of time that a team is free
  int numOfCustomers;
  int gold;
} team;


// Wardrobe synchronization methods
void enterWardrobe(fighter f);
void leaveWardrobe(fighter f);

// Helper methods
long getDiffTime(struct timeval *start, struct timeval *end);

// Linked list methods
void push(struct Node** head_ref, void *new_data, size_t data_size);

// Queue methods
GQueue* createQueue(unsigned capacity);
int isFull(GQueue* queue);
int isEmpty(struct Queue* queue);
void enqueue(GQueue* queue, int item);
int dequeue(GQueue* queue);
int front(GQueue* queue);
int rear(GQueue* queue);

#endif
