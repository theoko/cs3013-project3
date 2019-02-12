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

int isEmpty(struct Queue* queue) {

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
