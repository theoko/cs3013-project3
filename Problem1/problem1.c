#include "problem1.h"

/* * * * * * * * * * * * * * * * * * * * * * *
* Costume department initialization methods
* * * * * * * * * * * * * * * * * * * * *  * */

void initializeCostumeDepartment(int numOfPirates, int numOfNinjas, int numOfTeams) {

    pthread_mutex_init(&(costume_department.costume_mutex), NULL);

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

}



/* * * * * * * * * * * * * * * * * * * * * * *
* Costume department synchronization methods
* * * * * * * * * * * * * * * * * * * * * * * */

void enterCostumeDepartment(fighter_n *dresser) {

    if (costume_department.status == 0) {
        // Costume department is empty

        if(dresser->type == pirate) {

            pthread_mutex_lock(&(costume_department.costume_mutex));
            costume_department.status = 1;
            costume_department.count_in_department++;
            pthread_mutex_unlock(&(costume_department.costume_mutex));

            pthread_mutex_lock(&print_mutex);
            // Dress pirate
			printf("Dressing pirate %d\n", dresser->id);
			pthread_mutex_unlock(&print_mutex);

			int costuming_time = getNormDistro(pirateAvgCostumingTime);

            sleep(costuming_time);

            // Increment number of visits
			(*dresser).numOfVisits++;

			// Push time spent
			push(&(*dresser).visitTime, &costuming_time, sizeof(int));

            pthread_mutex_lock(&(costume_department.costume_mutex));
            costume_department.count_in_department--;
            costume_department.status = 0;

            costume_department.gross_revenue += costuming_time;
            pthread_mutex_unlock(&(costume_department.costume_mutex));

        } else {

            pthread_mutex_lock(&(costume_department.costume_mutex));
            costume_department.status = 2;
            costume_department.count_in_department++;
            pthread_mutex_unlock(&(costume_department.costume_mutex));

            pthread_mutex_lock(&print_mutex);
			// Dress ninja
			printf("Dressing ninja %d\n", dresser->id);

			pthread_mutex_unlock(&print_mutex);

			int costuming_time = getNormDistro(ninjaAvgCostumingTime);

            sleep(costuming_time);

            // Increment number of visits
			(*dresser).numOfVisits++;

			// Push time spent
			push(&(*dresser).visitTime, &costuming_time, sizeof(int));

		    pthread_mutex_lock(&(costume_department.costume_mutex));

            costume_department.count_in_department--;
            costume_department.status = 0;

            costume_department.gross_revenue += costuming_time;
            pthread_mutex_unlock(&(costume_department.costume_mutex));
        }
    } else if (costume_department.status == 1) {

        pthread_mutex_lock(&(costume_department.costume_mutex));

        // Costume department is occupied by pirate(s)
        costume_department.count_in_department++;
        pthread_mutex_unlock(&(costume_department.costume_mutex));

        pthread_mutex_lock(&print_mutex);
        printf("Dressing another pirate %d\n", dresser->id);

        pthread_mutex_unlock(&print_mutex);

        int costuming_time = getNormDistro(pirateAvgCostumingTime);

        sleep(costuming_time);

        // Increment number of visits
		(*dresser).numOfVisits++;

        // Push time spent
		push(&(*dresser).visitTime, &costuming_time, sizeof(int));

        pthread_mutex_lock(&(costume_department.costume_mutex));
        costume_department.count_in_department--;
        costume_department.status = 0;

        costume_department.gross_revenue += costuming_time;
        pthread_mutex_unlock(&(costume_department.costume_mutex));

    } else {

        pthread_mutex_lock(&(costume_department.costume_mutex));

        // Costume department is occupied by ninja(s)
        costume_department.count_in_department++;

        pthread_mutex_unlock(&(costume_department.costume_mutex));

        pthread_mutex_lock(&print_mutex);
	    printf("Dressing another ninja %d\n", dresser->id);

	    int costuming_time = getNormDistro(ninjaAvgCostumingTime);
        pthread_mutex_unlock(&print_mutex);

        sleep(costuming_time);

        // Increment number of visits
		(*dresser).numOfVisits++;

        // Push time spent
		push(&(*dresser).visitTime, &costuming_time, sizeof(int));

        pthread_mutex_lock(&(costume_department.costume_mutex));
        costume_department.count_in_department--;
        costume_department.status = 0;

        costume_department.gross_revenue += costuming_time;
        pthread_mutex_unlock(&(costume_department.costume_mutex));

    }

}

void leaveCostumeDepartment(fighter_n *dresser) {

  	double returning = drand48();

  	if(returning <= .25) { // 25% of saying yes
  		enterCostumeDepartment(dresser);
  	}

}

void *Action(void *dresser) {

    fighter_n *fn = (fighter_n *) dresser;

    if (fn->type == pirate) {

        printf("Started thread(pirate) with ID: %d\n", fn->id);

        int arrival_time = getNormDistro(pirateAvgArrivalTime);

        pthread_mutex_lock(&(costume_department.costume_mutex));
        costume_department.count_waiting++;
        pthread_mutex_unlock(&(costume_department.costume_mutex));

        // usleep for average arrival time of pirates before queueing
		sleep(arrival_time);

		pthread_mutex_lock(&(costume_department.costume_mutex));
		costume_department.count_waiting--;
		pthread_mutex_unlock(&(costume_department.costume_mutex));

        // Push time spent
		push(&(*fn).waitTime, &arrival_time, sizeof(int));

    } else if (fn->type == ninja) {

        printf("Started thread(ninja) with ID: %d\n", fn->id);

        int arrival_time = getNormDistro(ninjaAvgArrivalTime);

        pthread_mutex_lock(&(costume_department.costume_mutex));
		costume_department.count_waiting++;
		pthread_mutex_unlock(&(costume_department.costume_mutex));

        // usleep for average arrival time of ninjas before queueing
        sleep(arrival_time);

        pthread_mutex_lock(&(costume_department.costume_mutex));
		costume_department.count_waiting--;
		pthread_mutex_unlock(&(costume_department.costume_mutex));

        // Push time spent
		push(&(*fn).waitTime, &arrival_time, sizeof(int));

    }

    enterCostumeDepartment(fn); // Enter costume department
    leaveCostumeDepartment(fn); // Leave costume department

    return fn;

}

/* * * * * * * * * * * * * * * * * * * * * * *
* Helper methods
* * * * * * * * * * * * * * * * * * * * * * * */

// Generate random number from given mean
int getNormDistro(int mean) {
    float a = drand48();
    float b = drand48();
    float z = sqrt(-2 * log(a)) * cos(2 * M_PI * b); // Box-muller equation
    float num = ((mean / 2) * z) + mean;

    if (num < 1) { //check if rand result makes sense
        num = 1;
    }
//    if (num > 1000) {
//        num = 1000;
//    }

    return (int) floor(num); //turn float to int and round down
}

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
* Linked list methods
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

// Print list
void printList(GList *node, void (*fptr)(void *))
{
    while (node != NULL)
    {
        (*fptr)(node->data);
        node = node->next;
    }
}

// Function to print a float
void printInt(void *n)
{
   printf("%d\n", *(int *)n);
}

int sumOf(GList *node)
{
	int sum = 0;

	while(node != NULL)
	{
		sum += *(int *)	node->data;
		node = node->next;
	}

	return sum;
}

int check30Mins(GList *node)
{
	int total = 0;

	while(node != NULL)
	{
		int d = *(int *) node->data;

		if(d > 30) {
			total += d;
		}

		node = node->next;
	}

	return total;
}

int calculateTotalDebt(GList *node)
{
	int total = 0;

	while(node != NULL)
	{
		int d = *(int *) node->data;

		total += d;

		node = node->next;
	}

	return total;
}

/* * * * * * * * * * * * * * * * * * * * * * *
* Main()
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

  printf("\n In this world, pirateAvgCostumingTime = %d minutes\n", pirateAvgCostumingTime);
  printf(" In this world, pirateAvgArrivalTime = %d minutes\n", pirateAvgArrivalTime);
  printf(" In this world, ninjaAvgCostumingTime = %d minutes\n", ninjaAvgCostumingTime);
  printf(" In this world, ninjaAvgArrivalTime = %d minutes\n\n", ninjaAvgArrivalTime);

  pthread_mutex_init(&print_mutex, NULL);

  pthread_t pirates[numOfPirates];
  pthread_t ninjas[numOfNinjas];

  // Initialize costume department
  initializeCostumeDepartment(numOfPirates, numOfNinjas, numOfTeams);

  // Start timer
  gettimeofday(&(costume_department.start),  NULL);

//  for(int day = 0; day < numOfTeams; day++) {

	  	// Pointer to pirate struct
	    fighter_n *pn[numOfPirates];
	    for(i = 0; i < numOfPirates; i++) {

	        pn[i] = (fighter_n *) malloc(sizeof(fighter_n)); // TODO: add free in the end of main to avoid potential memory leak :ppppp
	        if(!pn[i]) {
	          printf("Cannot malloc() for pirate!!!!!\n");
	          return -1;
	        }

	        pn[i]->id = i;
	        pn[i]->owes = 0;
	        pn[i]->type = pirate;
	        pn[i]->numOfVisits = 0;

	        pthread_create(&pirates[i], NULL, Action, (void *) pn[i]); // TODO: Action
	    }

	    // Pointer to ninja struct
	    fighter_n *nn[numOfNinjas];
	    for(i = 0; i < numOfNinjas; i++) {

	        nn[i] = (fighter_n *) malloc(sizeof(fighter_n)); // TODO: add free in the end of main to avoid potential memory leak :ppppp
	        if(!nn[i]) {
	          printf("Cannot malloc() for ninja!!!!!\n");
	          return -1;
	        }

	        nn[i]->id = i;
	        nn[i]->type = ninja;
	        nn[i]->numOfVisits = 0;

	        pthread_create(&ninjas[i], NULL, Action, (void *) nn[i]); // Same as above for Action
	    }

	    printf("Calling join()\n");

	    for(i = 0; i < numOfPirates; i++) {
	          pthread_join(pirates[i], NULL);
	    }

	    for(i = 0; i < numOfNinjas; i++) {
	          pthread_join(ninjas[i], NULL);
	    }

	    printf("\nVisit times for pirates:\n");
	    for(i = 0; i < numOfPirates; i++) {
	      pn[i]->owes =  calculateTotalDebt(pn[i]->visitTime);

	  	  printf("Pirate %d: ", i);
	  	  printList(pn[i]->visitTime, printInt);
	    }

	    printf("\nVisit times for ninjas:\n");
	    for(i = 0; i < numOfNinjas; i++) {
	      nn[i]->owes =  calculateTotalDebt(nn[i]->visitTime);

	  	  printf("Ninja %d: ", i);
	    	  printList(nn[i]->visitTime, printInt);
	    }

	    printf("\nWait times for pirates:\n");
		for(i = 0; i < numOfPirates; i++) {

//		  int totalWait = check30Mins(pn[i]->waitTime);

//		  costume_department.gross_revenue = costume_department.gross_revenue - totalWait;

		  printf("Pirate %d: ", i);
		  printList(pn[i]->waitTime, printInt);
		}

		printf("\nWait times for ninjas:\n");
		for(i = 0; i < numOfNinjas; i++) {

//		  int totalWait = check30Mins(nn[i]->waitTime);

//		  costume_department.gross_revenue = costume_department.gross_revenue - totalWait;

		  printf("Ninja %d: ", i);
			  printList(nn[i]->waitTime, printInt);
		}

		printf("\nDebt for pirates:\n");
		for(i = 0; i < numOfPirates; i++) {
		  printf("Pirate %d owes: %d\n", i, pn[i]->owes);
		}

		printf("\nDebt for ninjas:\n");
		for(i = 0; i < numOfNinjas; i++) {
		  printf("Ninja %d owes: %d\n", i, nn[i]->owes);
		}



//  }

  // Pay teams
  costume_department.expenses = 5 * numOfTeams;
  costume_department.total_profits = costume_department.gross_revenue - costume_department.expenses;

  // Stop timer
  gettimeofday(&(costume_department.end),  NULL);

  // Print Statistics
  // 	- Print debt of each pirate (1 gold piece for every minute inside -- free for wait time > 30 mins)!
  //    - Print debt of each ninja !
  //    - Print sum of debt of pirates
  //    - Print sum of debt of ninjas
  //    - Print visit times and wait times of pirates & ninjas
  //    - Print costume department expenses & profits (expenses: 5 gold pieces / team, profits: revenue - expenses)
  //    - Print gold-per-visit for pirates/ninjas
  //    - Print amount of time that each team was busy
  //    - Print amount of time that each team was free
  //    - Print average queue length

  printf("\nGold-per-visit for pirates:\n");
	for(i = 0; i < numOfPirates; i++) {
	  if(pn[i]->numOfVisits != 0)
	  printf("Pirate %d gold-per-visit: %f\n", i, (float) pn[i]->owes / pn[i]->numOfVisits);
	}

	printf("\nGold-per-visit for ninjas:\n");
	for(i = 0; i < numOfNinjas; i++) {
	  if(nn[i]->numOfVisits != 0)
	  printf("Ninja %d gold-per-visit: %f\n", i, (float) nn[i]->owes / nn[i]->numOfVisits);
	}

  int debtPirates = 0;
  for(i = 0; i < numOfPirates; i++)
	  debtPirates += pn[i]->owes;

  int debtNinjas = 0;
  for(i = 0; i < numOfPirates; i++)
  	  debtNinjas += nn[i]->owes;

  printf("\n\nSum of debt for pirates: %d", debtPirates);

  printf("\nSum of debt for ninjas: %d\n", debtNinjas);

  printf("\nCostume department gross revenue: %d", costume_department.gross_revenue);

  printf("\nCostume department profit: %d", costume_department.total_profits);

  printf("\nCostume department expenses: %d\n", costume_department.expenses);

  pthread_mutex_destroy(&(costume_department.costume_mutex));
  pthread_mutex_destroy(&print_mutex);

  return 0;

}
