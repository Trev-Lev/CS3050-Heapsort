#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUSTOMERS 100
#define MAX_CHARACTERS 20

typedef struct person {
    char* name;
    int minutes;
} Person;

Person* importPeople(char* filename);
void freeThePeople(Person* people);
void heapifyString(Person* people, int i, int size);
int left(int i);
int right(int i);
int heapSize(Person* person);
char* rightString(char* names, int i);
char* leftString(char* names, int i);
void heapSortString(Person* people);
void buildStringHeap(Person* people, int size);
void heapSortInteger(Person* people);
void buildIntHeap(Person* people, int size);
void heapifyInt(Person* people, int i, int size);
void writeToFile(Person* people, FILE *fp);
void printPeople(Person* people);

int size = 0;

int main(int argc, char* argv[])
{

    Person* people = importPeople(argv[1]);
    FILE *fp = fopen(argv[2], "w");
    // Print inputs just to make sure
    printf("This is the input:\n");
    //fprintf(fp, "This is the input:\n");

    printPeople(people);
    printf("\n\n");
    //writeToFile(people, fp);
    //fprintf(fp, "\n");

    // Sort names
    heapSortString(people);

    printf("We have the following customers (sorted by their names):\n");
    fprintf(fp, "We have the following customers (sorted by their names):\n\n");

    printPeople(people);
    writeToFile(people, fp);

    // Sort by number, now
    heapSortInteger(people);

    printf("Their service order should be:\n");
    fprintf(fp, "\nTheir service order should be:\n");
    printPeople(people);
    writeToFile(people, fp);

    // The total waiting time calculated here
    // I did not feel like making this its own function
    int i;
    int totalWaitingTime = 0;
    int waitingTime = 0;
    int arraySize = size;
    size++; // To get the real size
    for (i = 0; i <= arraySize; i++) {
        waitingTime = people[i].minutes * size;
        totalWaitingTime += waitingTime;
        size--; // It is okay to decrement size here, the program will end shortly after
    }

    printf("\nThe total waiting time of all customers will be %d\n", totalWaitingTime);
    fprintf(fp, "\nThe total waiting time of all the customers will be %d\n", totalWaitingTime);
    // Free memory
    fclose(fp);
    freeThePeople(people);
    return 0;
}

void printPeople(Person* people) {
    int i;
    for (i = 0; i <= size; i++) {
        printf("%s %d\n", people[i].name, people[i].minutes);
    }
}

void writeToFile(Person* people, FILE *fp) {
    int i;
    for (i = 0; i <= size; i++) {
        fprintf(fp, "%s %d\n", people[i].name, people[i].minutes);
    }
}

void heapifyInt(Person* people, int i, int size) {

    int l, r, largest;
    l = left(i);
    r = right(i);
    largest = i;
    Person person;

    if (l <= size && people[l].minutes > people[largest].minutes) {
        largest = l;
    }
    if (r <= size && people[r].minutes > people[largest].minutes) {
        largest = r;
    }
    // If the index has changed
    if (largest != i) {
        person.name = malloc(sizeof(char) * MAX_CHARACTERS);
        // Swap people
        strcpy(person.name, people[i].name);
        person.minutes = people[i].minutes;

        strcpy(people[i].name, people[largest].name);
        people[i].minutes = people[largest].minutes;

        strcpy(people[largest].name, person.name);
        people[largest].minutes = person.minutes;

        free(person.name);
        // Recursion
        heapifyString(people, largest, size);


    }
}

void buildIntHeap(Person* people, int size) {
    int i;
    // size-1/2 ?
    for (i = size/2; i >= 0; i--) {
        heapifyInt(people, i, size);
    }
}

void heapSortInteger(Person* people) {
    buildIntHeap(people, size);
    Person person;
    person.name = malloc(sizeof(char) * MAX_CHARACTERS);
    int i;

    printf("\nThis is the integer based heap:\n");
    printf("%s %d\n", people[0].name, people[0].minutes);
    printf("%s %d\n", people[1].name, people[1].minutes);
    printf("%s %d\n", people[2].name, people[2].minutes);
    printf("%s %d\n", people[3].name, people[3].minutes);
    printf("%s %d\n", people[4].name, people[4].minutes);
    printf("\n");

    for (i = size; i > 0; i--) {

        person.name = malloc(sizeof(char) * MAX_CHARACTERS);
        // GOAL: Swap 1st element with element of i, decrease the size
        // Call "heapify" with array, 1st element, size to maintain heap
        // Swap last element with first element

        strcpy(person.name, people[0].name);
        person.minutes = people[0].minutes;

        strcpy(people[0].name, people[i].name);
        people[0].minutes = people[i].minutes;

        strcpy(people[i].name, person.name);
        people[i].minutes = person.minutes;


        free(person.name);
        // Reform the heap
        heapifyInt(people, 0, i-1);

    }
}

void heapSortString(Person* people) {

    buildStringHeap(people, size);
    Person person; // Temp
    person.name = malloc(sizeof(char) * MAX_CHARACTERS);
    int i;

    printf("This is the initial heap:\n");
    printf("%s %d\n", people[0].name, people[0].minutes);
    printf("%s %d\n", people[1].name, people[1].minutes);
    printf("%s %d\n", people[2].name, people[2].minutes);
    printf("%s %d\n", people[3].name, people[3].minutes);
    printf("%s %d\n", people[4].name, people[4].minutes);
    printf("\n");

    for (i = size; i > 0; i--) {
        person.name = malloc(sizeof(char) * MAX_CHARACTERS);
        // GOAL: Swap 1st element with element of i, decrease the size
        // Call "heapify" with array, 1st element, size to maintain heap
        // Swap last element with first element

        //person = people[i];

        strcpy(person.name, people[0].name);
        person.minutes = people[0].minutes;

        strcpy(people[0].name, people[i].name);
        people[0].minutes = people[i].minutes;

        strcpy(people[i].name, person.name);
        people[i].minutes = person.minutes;

        free(person.name);
        // Reform the heap
        heapifyString(people, 0, i-1);
    }


}

void buildStringHeap(Person* people, int size) {
    int i;
    // size-1/2 ?
    for (i = size/2; i >= 0; i--) {
        heapifyString(people, i, size);
    }
}

void heapifyString(Person* people, int i, int size) {

    //printf("i is %d\n", i);

    int l, r, largest;
    Person person;
    // These are the index numbers
    l = left(i);
    r = right(i);
    largest = i;

    // < 0 means that str1 is less than str2 = string 1 comes before in the alphabet
    // > 0 is what we want for heap sort

    // Left side of internal node
    if (l <= size && strcmp(people[l].name, people[largest].name) > 0) {
        largest = l;
    }

    // Right side of internal node
    if (r <= size && strcmp(people[r].name, people[largest].name) > 0) {
        largest = r;
    }

    // If the index has changed
    // If largest == l or largest == r
    if (largest != i) {
        person.name = malloc(sizeof(char) * MAX_CHARACTERS);

        // Swap people
        strcpy(person.name, people[i].name);
        person.minutes = people[i].minutes;

        strcpy(people[i].name, people[largest].name);
        people[i].minutes = people[largest].minutes;

        strcpy(people[largest].name, person.name);
        people[largest].minutes = person.minutes;

        free(person.name);
        // Recursion
        heapifyString(people, largest, size);

    }

}

int right(int i) {
    int right = 2*i + 2;
    return right;
}

int left(int i) {
    int left = 2*i + 1;
    return left;
}

void freeThePeople(Person* people) {
    int i;
    for (i = 0; i <= size; i++) {
        free((people+i)->name);
    }
    free(people);
}

Person* importPeople(char* filename) {
    FILE *fp = fopen(filename, "r");
    // Error checking
    if (fp == NULL) {
        printf("File could not be opened.\n");
        printf("%s", filename);
        return NULL;
    }
    // Dynamic memory allocation for the customers
    Person* people = malloc(sizeof(Person) * MAX_CUSTOMERS);
    // To hold the integer value of minutes
    Person person;
    int i;
    // Scan names and minutes
    for (i = 0; !feof(fp); i++) {
        // Dynamic memory allocation for the names of the customers
        (people+i)->name = malloc(sizeof(char) * MAX_CHARACTERS);
        fscanf(fp, "%s", (people+i)->name);
        fscanf(fp, "%d", &person.minutes);
        // Reassign
        (people+i)->minutes = person.minutes;
        // Bump the size of the heap by 1
        // The heap has not been built yet, but this is
        // one way to track its size.
        size++;
    }
    size--;
    // Return the loaded array of people and close the file pointer
    fclose(fp);
    return people;
}
