#include "problem1.h"

/* * * * * * * * * * * * * * * * * * * * * * *
*				Wardrobe synchronization methods
* * * * * * * * * * * * * * * * * * * * * * * */

void enterWardrobe(fighter f) {

}

void leaveWardrobe(fighter f) {

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

/* * * * * * * * * * * * * * * * * * * * * * *
*							Linked list methods
* * * * * * * * * * * * * * * * * * * * * * * */

void push(GList** head_ref, void *new_data, size_t data_size)
{
    // Allocate memory for node
    GList* new_node = (struct Node*)malloc(sizeof(GList));

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

int isEmpty(struct GQueue* queue) {

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

void printArgErrorInfo() {
	printf("./pn teams pirates ninjas pirateAvgCostumingTime ninjaAvgCostumingTime pirateAvgArrivalTime ninjaAvgArrivalTime\n");
}

int main(int argc, int *argv[]) {
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



}
