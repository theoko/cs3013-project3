#include<stdio.h>
#include<stdlib.h>


// sem_t pirates[N];
// sem_t ninjas[N];


void printArgErrorInfo() {
	printf("./pn teams pirates ninjas pirateAvgCostumingTime ninjaAvgCostumingTime pirateAvgArrivalTime ninjaAvgArrivalTime\n");
}

int runWardrobe() {
	
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
