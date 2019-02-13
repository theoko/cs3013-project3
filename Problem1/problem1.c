#include "problem1.h"

/* * * * * * * * * * * * * * * * * * * * * * *
*				Wardrobe initialization methods
* * * * * * * * * * * * * * * * * * * * * * * */

void initializeCostumeDepartment(int numOfPirates, int numOfNinjas, int numOfTeams) {

    pthread_mutex_init(&(costume_department->costume_mutex), NULL);
    pthread_cond_init(&(costume_department->costume_condition), NULL);
    costume_department->status = 0;

    costume_department->count_in_department = 0;
    costume_department->count_waiting = 0;

    costume_department->count_gold_ninjas = 0;
    costume_department->count_gold_pirates = 0;

    costume_department->average_queue_length = 0.0; // Add numOfCustomers and divide by numOfTeams

    costume_department->expenses = 0; // Each costuming team costs 5 gold per day

    costume_department->gross_revenue = 0;
    costume_department->gold_per_visit = 0;
    costume_department->total_profits = 0;

    gettimeofday(&(costume_department->start),  NULL);

    pirate_queue = createQueue(numOfPirates);
    ninja_queue = createQueue(numOfNinjas);

    // if (numOfTeams == 3) {
    //   mixed_queue = createQueue(pirate_queue->capacity / 3);
    // }
}



/* * * * * * * * * * * * * * * * * * * * * * *
*				Wardrobe synchronization methods
* * * * * * * * * * * * * * * * * * * * * * * */

void enterWardrobe(fighter f) {

}

void leaveWardrobe(fighter f) {

}

void *Action(void *dresser) {

}

/* * * * * * * * * * * * * * * * * * * * * * *
*								Helper methods
* * * * * * * * * * * * * * * * * * * * * * * */

long getDiffTime(struct timeval *start, struct timeval *end) {
    unsigned long long diff = 0;
    unsigned long long sec = 0;
    unsigned long long ms = 0;

    sec = end->tv_sec - start->tv_sec; //get number of seconds
    diff += sec * 1000; //convert to milliseconds

    if (start->tv_usec > end->tv_usec) {
        ms = start->tv_usec - end->tv_usec; //microseconds
        diff += ms / 1000; //convert to milliseconds
    } else {
        ms = end->tv_usec - start->tv_usec;
        diff += ms / 1000; //conver to milliseconds
    }

    return diff;
}

void printArgErrorInfo() {
	printf("./pn teams pirates ninjas pirateAvgCostumingTime ninjaAvgCostumingTime pirateAvgArrivalTime ninjaAvgArrivalTime\n");
}

void printErrorForEntity(char *entityName, error_type err) {
  if(err == correct_num) {
    printf("Not appropriate number of %s inputted\n", entityName);
  } else if (err == invalid_costuming_time) {
    printf("Average costuming time for %s cannot exceed 1440 seconds\n", entityName);
  } else if (err == invalid_arrival_time) {
    printf("Average arrival time for %s cannot exceed 1440 seconds\n", entityName);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * *
*							Linked list methods
* * * * * * * * * * * * * * * * * * * * * * * */

void push(GList** head_ref, void *new_data, size_t data_size)
{
    // Allocate memory for node
    GList* new_node = (GList*) malloc(sizeof(GList));

    new_node->data  = malloc(data_size);
    new_node->next = (*head_ref);

    // Copy contents of new_data to newly allocated memory.
    // Assumption: char takes 1 byte.
    int i;
    for (i=0; i<data_size; i++)
        *(char *)(new_node->data + i) = *(char *)(new_data + i);

    // Change head pointer as new node is added at the beginning
    (*head_ref)    = new_node;
}

/* * * * * * * * * * * * * * * * * * * * * * *
*								Queue methods
* * * * * * * * * * * * * * * * * * * * * * * */

GQueue* createQueue(unsigned capacity) {

		// Allocate space for GQueue struct
	  GQueue* queue = (GQueue*) malloc(sizeof(GQueue));

		// Set max queue capacity
	  queue->capacity = capacity;

		// Set queue front
	  queue->front = queue->size = 0;

		// Set queue rear
	  queue->rear = capacity - 1;

	  queue->array = (int*) malloc(queue->capacity * sizeof(int));

	  return queue;
}

int isFull(GQueue* queue) {

		// Check if size has reached max queue capacity
		return (queue->size == queue->capacity);

}

int isEmpty(GQueue* queue) {

		// Check if queue size is 0
		return (queue->size == 0);

}

void enqueue(GQueue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}

int dequeue(GQueue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

int front(GQueue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

int rear(GQueue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

/* * * * * * * * * * * * * * * * * * * * * * *
*									Main()
* * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char **argv) {
	if(argc < 2) {
					printf("Missing number of costuming teams.\n");
					printArgErrorInfo();
					return -1;
	}

	if(argc < 3) {
					printf("Missing number of pirates.\n");
          printArgErrorInfo();
          return -1;
	}

	if(argc < 4) {
          printf("Missing number of ninjas.\n");
          printArgErrorInfo();
          return -1;
  }

	if(argc < 5) {
          printf("Missing pirate average costuming time.\n");
          printArgErrorInfo();
          return -1;
  }

	if(argc < 6) {
          printf("Missing ninja average costuming time.\n");
          printArgErrorInfo();
          return -1;
  }

	if(argc < 7) {
          printf("Missing pirate average arrival time.\n");
          printArgErrorInfo();
          return -1;
  }

	if(argc < 8) {
          printf("Missing ninja average arrival time.\n");
          printArgErrorInfo();
          return -1;
  }

  // Error types
  error_type correct_n = correct_num;
  error_type invalid_costuming_t = invalid_costuming_time;
  error_type invalid_arrival_t = invalid_arrival_time;

  int total_err = 0;
  unsigned int i;

  unsigned int numOfTeams = (unsigned) atoi(argv[1]);
  if(numOfTeams < 2 || numOfTeams > 4) {
      printErrorForEntity("teams", correct_n);
      total_err++;
  }

  unsigned int numOfPirates = (unsigned) atoi(argv[2]);
  if(numOfPirates < 10 || numOfPirates > 50) {
      printErrorForEntity("pirates", correct_n);
      total_err++;
  }

  unsigned int numOfNinjas = (unsigned) atoi(argv[3]);
  if(numOfNinjas < 10 || numOfNinjas > 50) {
      printErrorForEntity("ninjas", correct_n);
      total_err++;
  }

  int pirateAvgCostumingTime = atoi(argv[4]);
  if(pirateAvgCostumingTime > 1440) {
      printErrorForEntity("pirates", invalid_costuming_t);
      total_err++;
  }

  int ninjaAvgCostumingTime = atoi(argv[5]);
  if(ninjaAvgCostumingTime > 1440) {
      printErrorForEntity("ninjas", invalid_costuming_t);
      total_err++;
  }

  int pirateAvgArrivalTime = atoi(argv[6]);
  if(pirateAvgArrivalTime > 1440) {
      printErrorForEntity("pirates", invalid_arrival_t);
      total_err++;
  }

  int ninjaAvgArrivalTime = atoi(argv[7]);
  if(ninjaAvgArrivalTime > 1440) {
      printErrorForEntity("ninjas", invalid_arrival_t);
      total_err++;
  }

  if(total_err > 0) {
      printf("Exitting with %d error(s)\n", total_err);
      return -1;
  }

  pthread_mutex_init(&print_mutex, NULL);

  pthread_t pirates[numOfPirates];
  pthread_t ninjas[numOfNinjas];

  // Initialize wardrobe
  initializeCostumeDepartment(numOfPirates, numOfNinjas, numOfTeams);

  for(i = 0; i < numOfPirates; i++) {

      pirate_n *pirate = malloc(sizeof(pirate_n *)); // TODO: add free in the end of main to avoid potential memory leak :ppppp
      pirate->id = i;
      pthread_create(&pirates[i], NULL, Action, (void *) pirate); // TODO: Action
  }

  for(i = 0; i < numOfNinjas; i++) {

      ninja_n *ninja = malloc(sizeof(ninja_n *)); // TODO: add free in the end of main to avoid potential memory leak :ppppp
      ninja->id = i;
      pthread_create(&ninjas[i], NULL, Action, (void *) ninja); // Same as above for Action
  }

  for(i = 0; i < numOfPirates; i++) {
      pthread_join(&pirates[i], NULL);
  }

  for(i = 0; i < numOfNinjas; i++) {
      pthread_join(&ninjas[i], NULL);
  }

  //TODO: Function that will finalize functionality of program (print some requested
  //      stats, get time when program is about to finish etc.)

  return 0;

}
