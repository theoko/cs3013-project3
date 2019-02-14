#include "problem1.h"

/* * * * * * * * * * * * * * * * * * * * * * *
* Costume department initialization methods
* * * * * * * * * * * * * * * * * * * * * * * */

void initializeCostumeDepartment(int numOfPirates, int numOfNinjas, int numOfTeams) {

    pthread_mutex_init(&(costume_department.costume_mutex), NULL);
    pthread_cond_init(&(costume_department.costume_condition), NULL);
    condition = 0;

    costume_department.status = 0;

    costume_department.count_in_department = 0;
    costume_department.count_waiting = 0;

    costume_department.count_gold_ninjas = 0;
    costume_department.count_gold_pirates = 0;

    costume_department.average_queue_length = 0.0; // Add numOfCustomers and divide by numOfTeams

    costume_department.expenses = 0; // Each costuming team costs 5 gold per day

    costume_department.gross_revenue = 0;
    costume_department.gold_per_visit = 0;
    costume_department.total_profits = 0;

    gettimeofday(&(costume_department.start),  NULL);

    pirate_queue = createQueue(numOfPirates);
    ninja_queue = createQueue(numOfNinjas);

}



/* * * * * * * * * * * * * * * * * * * * * * *
* Costume department synchronization methods
* * * * * * * * * * * * * * * * * * * * * * * */

void enterCostumeDepartment(fighter_n *dresser) {

    condition = 1;

    /* CRITICAL SECTION */
    if (costume_department.status == 0) {
        // Costume department is empty
        double entityProb = drand48();

        if(entityProb < .5 && !isEmpty(pirate_queue)) {

            // int tmp_pirate = dequeue(pirate_queue);
            // while (tmp_pirate != dresser->id) {
            //     enqueue(pirate_queue, tmp_pirate);
            //
            //     tmp_pirate = dequeue(pirate_queue);
            // }

            costume_department.status = 1;
            costume_department.count_in_department++;

            // Dress pirate
            printf("Dressing pirate %d\n", dresser->id);
            sleep(pirateAvgCostumingTime);
            costume_department.count_in_department--;
	    costume_department.status = 0;
        } else {
            if(isEmpty(ninja_queue))
                return;

            // int tmp_ninja = dequeue(ninja_queue);
            // while (tmp_ninja != dresser->id) {
            //     enqueue(ninja_queue, tmp_ninja);
            //
            //     tmp_ninja = dequeue(pirate_queue);
            // }

            costume_department.status = 2;
            costume_department.count_in_department++;

            // Dress ninja
            printf("Dressing ninja %d\n", dresser->id);
            sleep(ninjaAvgCostumingTime);
            costume_department.count_in_department--;
	    costume_department.status = 0;
        }
    } else if (costume_department.status == 1 && !isEmpty(pirate_queue)) {

        // int tmp_pirate = dequeue(pirate_queue);
        // while (tmp_pirate != dresser->id) {
        //     enqueue(pirate_queue, tmp_pirate);
        //
        //     tmp_pirate = dequeue(pirate_queue);
        // }

        // Costume department is occupied by pirate(s)
        // Dequeue another pirate
        costume_department.count_in_department++;

        printf("Dressing another pirate %d\n", dresser->id);
        sleep(pirateAvgCostumingTime);
        costume_department.count_in_department--;
        costume_department.status = 0;
    } else {
        if(isEmpty(ninja_queue))
            return;

        // int tmp_ninja = dequeue(ninja_queue);
        // while (tmp_ninja != dresser->id) {
        //     enqueue(ninja_queue, tmp_ninja);
        //
        //     tmp_ninja = dequeue(pirate_queue);
        // }

        // Costume department is occupied by ninja(s)
        // Dequeue another ninja
        costume_department.count_in_department++;

        printf("Dressing another ninja %d\n", dresser->id);
        sleep(ninjaAvgCostumingTime);
        costume_department.count_in_department--;
        costume_department.status = 0;
    }
    /* END OF CRITICAL SECTION */

    condition = 0;
}

void leaveCostumeDepartment(fighter_n *dresser) {

  /* CRITICAL SECTION */

  double returning = drand48();

  // Check if dresser is returning
  if(returning <= .25) { // 25% of saying yes
      if (dresser->type == pirate) {
          enqueue(pirate_queue, dresser->id);
      } else if (dresser->type == ninja) {
          enqueue(ninja_queue, dresser->id);
      }
  }
  /* END OF CRITICAL SECTION */

}

void *Action(void *dresser) {

    fighter_n *fn = (fighter_n *) dresser;

    if (fn->type == pirate) {
        printf("Started thread(pirate) with ID: %d\n", fn->id);

        // Sleep for average arrival time of pirates before queueing
        sleep(pirateAvgArrivalTime);

        // Enqueue pirate
        enqueue(pirate_queue, fn->id);
    } else if (fn->type == ninja) {
        printf("Started thread(ninja) with ID: %d\n", fn->id);

        // Sleep for average arrival time of ninjas before queueing
        sleep(ninjaAvgArrivalTime);

        // Enqueue ninja
        enqueue(ninja_queue, fn->id);
    }



    //if((costume_department.status == 1 && fn->type == pirate) || (costume_department.status == 2 && fn->type == ninja)) {

      //if (fn->type == ninja)
      //  printf("Ninja %d: Entering costume department together with other ninja(s)\n", fn->id);
      //else
      //  printf("Pirate %d: Entering costume department together with other pirate(s)\n", fn->id);

      //enterCostumeDepartment(fn);
      //leaveCostumeDepartment(fn);

      //if (fn->type == ninja)
      //  printf("Ninja %d: Leaving costume department\n", fn->id);
      //else
      //  printf("Pirate %d: Leaving costume department\n", fn->id);

    //} else {

    pthread_mutex_lock(&(costume_department.costume_mutex));

    // Acquire the lock
    if (fn->type == ninja)
      printf("Ninja %d: Acquiring lock\n", fn->id);
    else
      printf("Pirate %d: Acquiring lock\n", fn->id);

    //while(!condition) {
	//printf("Waiting...\n");
    	//pthread_cond_wait(&(costume_department.costume_condition), &(costume_department.costume_mutex));
    //}

    enterCostumeDepartment(fn); // Enter costume department
    leaveCostumeDepartment(fn); // Leave costume department

    if (fn->type == ninja)
      printf("Ninja %d: Releasing lock\n", fn->id);
    else
      printf("Pirate %d: Releasing lock\n", fn->id);

    pthread_mutex_unlock(&(costume_department.costume_mutex));

    pthread_cond_signal(&(costume_department.costume_condition));

    //}


    return fn;

}

/* * * * * * * * * * * * * * * * * * * * * * *
* Helper methods
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
*	Linked list methods
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
*	Queue methods
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
*	Main()
* * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char **argv) {

  srand48(SEED_VAL);

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

  numOfTeams = (unsigned) atoi(argv[1]);
  if(numOfTeams < 2 || numOfTeams > 4) {
      printErrorForEntity("teams", correct_n);
      total_err++;
  }

  numOfPirates = (unsigned) atoi(argv[2]);
  if(numOfPirates < 10 || numOfPirates > 50) {
      printErrorForEntity("pirates", correct_n);
      total_err++;
  }

  numOfNinjas = (unsigned) atoi(argv[3]);
  if(numOfNinjas < 10 || numOfNinjas > 50) {
      printErrorForEntity("ninjas", correct_n);
      total_err++;
  }

  pirateAvgCostumingTime = atoi(argv[4]);
  if(pirateAvgCostumingTime > 1440) {
      printErrorForEntity("pirates", invalid_costuming_t);
      total_err++;
  }

  ninjaAvgCostumingTime = atoi(argv[5]);
  if(ninjaAvgCostumingTime > 1440) {
      printErrorForEntity("ninjas", invalid_costuming_t);
      total_err++;
  }

  pirateAvgArrivalTime = atoi(argv[6]);
  if(pirateAvgArrivalTime > 1440) {
      printErrorForEntity("pirates", invalid_arrival_t);
      total_err++;
  }

  ninjaAvgArrivalTime = atoi(argv[7]);
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

  // Initialize costume department
  initializeCostumeDepartment(numOfPirates, numOfNinjas, numOfTeams);

  for(i = 0; i < numOfPirates; i++) {

      fighter_n *pn = (fighter_n *) malloc(sizeof(fighter_n)); // TODO: add free in the end of main to avoid potential memory leak :ppppp
      if(!pn) {
        printf("Cannot malloc() for pirate!!!!!\n");
        return -1;
      }

      (*pn).id = i;
      (*pn).type = pirate;

      pthread_create(&pirates[i], NULL, Action, (void *) pn); // TODO: Action
  }

  for(i = 0; i < numOfNinjas; i++) {

      fighter_n *nn = (fighter_n *) malloc(sizeof(fighter_n)); // TODO: add free in the end of main to avoid potential memory leak :ppppp
      if(!nn) {
        printf("Cannot malloc() for ninja!!!!!\n");
        return -1;
      }

      (*nn).id = i;
      (*nn).type = ninja;

      pthread_create(&ninjas[i], NULL, Action, (void *) nn); // Same as above for Action
  }

  printf("Calling join() for pirates.\n");
  while(!isEmpty(pirate_queue))
      pthread_join(pirates[dequeue(pirate_queue)], NULL);
  printf("Pirates done.\n");

  printf("Calling join() for ninjas.\n");
  while(!isEmpty(ninja_queue))
      pthread_join(ninjas[dequeue(ninja_queue)], NULL);
  printf("Ninjas done.\n");

  pthread_mutex_destroy(&(costume_department.costume_mutex));
  pthread_mutex_destroy(&print_mutex);

  //TODO: Function that will finalize functionality of program (print some requested
  //      stats, get time when program is about to finish etc.)

  return 0;

}
