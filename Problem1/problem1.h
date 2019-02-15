#ifndef PROBLEM1_H
#define PROBLEM1_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <limits.h>

#define SEED_VAL (100)

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

typedef enum err_t {
  correct_num, invalid_costuming_time, invalid_arrival_time
} error_type;

typedef struct fighter_node {
  int id; // number to help identify corresponding thread
  int owes; // Amount of gold owed
  int numOfVisits; // Number of visits
  GList* visitTime; // List which will keep track of time spent on each visit -- minutes!!
  GList* waitTime; // List which will keep track of time spent waiting -- minutes!!
  fighter type;
} fighter_n;

// // Ninja:
// //  - owes
// //  - number of visits
// //  - visit time
// //  - time waiting
// typedef struct ninja_node {
//   int id; // number to help identify corresponding thread
//   int owes; // Amount of gold owed
//   int numOfVisits; // Number of visits
//   GList* visitTime; // List which will keep track of time spent on each visit
//   GList* waitTime; // List which will keep track of time spent waiting
//   fighter type;
// } ninja_n;
//
// // Pirate:
// //  - owes
// //  - number of visits
// //  - time waiting
// typedef struct pirate_node {
//   int id; // number to help identify corresponding thread
//   int owes; // Amount of gold owed
//   int numOfVisits;
//   GList* visitTime; // List which will keep track of time spent on each visit
//   GList* waitTime; // List which will keep track of time spent waiting
//   fighter type;
// } pirate_n;

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
} team_n;

// Costume department:
//   - costume_mutex
//   - costume_condition
//   - status:
//        -- 0: empty
//        -- 1: pirate
//        -- 2: ninja
typedef struct costume_d {
  pthread_mutex_t costume_mutex;
  pthread_cond_t costume_condition;

  int status;
  int count_in_department;
  int count_waiting;

  int count_gold_ninjas;
  int count_gold_pirates;

  float average_queue_length; // Add numOfCustomers and divide by numOfTeams

  int expenses; // Each costuming team costs 5 gold per day

  int gross_revenue;
  int gold_per_visit;
  int total_profits;

  struct timeval start;
  struct timeval end;

} costume_dept;

// Wardrobe initialization methods
void initializeCostumeDepartment(int numOfPirates, int numOfNinjas, int numOfTeams);

// Wardrobe synchronization methods
void enterCostumeDepartment(fighter_n *dresser);
void leaveCostumeDepartment(fighter_n *dresser);

// Helper methods
int getNormDistro(int mean);
long getDiffTime(struct timeval *start, struct timeval *end);
void printArgErrorInfo();
void printErrorForEntity(char *entityName, error_type err);

// Linked list methods
void push(GList** head_ref, void *new_data, size_t data_size);
void printList(GList *node, void (*fptr)(void *));
void printInt(void *n);

// Queue methods
GQueue* createQueue(unsigned capacity);
int isFull(GQueue* queue);
int isEmpty(GQueue* queue);
void enqueue(GQueue* queue, int item);
int dequeue(GQueue* queue);
int front(GQueue* queue);
int rear(GQueue* queue);

// Global variables
costume_dept costume_department; // Struct that holds the costume department properties

GQueue* pirate_queue; // Queue to hold pirates
GQueue* ninja_queue; // Queue to hold ninjas
// DON'T NEED A MIXED_QUEUE

pthread_mutex_t print_mutex; // Mutex for printing
int condition;

unsigned int numOfTeams;
unsigned int numOfPirates;
unsigned int numOfNinjas;
int pirateAvgCostumingTime;
int ninjaAvgCostumingTime;
int pirateAvgArrivalTime;
int ninjaAvgArrivalTime;

#endif
