/* CS 212 Project: Donations
   */

#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>  


#define QUEUE_SIZE 12  
#define BLOOD_TYPES 4
#define LINE_LENGTH 80



struct queue
{
  /* Add your struct members here. */
  int front, back, population;
  char *strings[QUEUE_SIZE];
};

typedef struct queue Queue;


void initializeQueue(Queue *q){
	q->front = 0;
	q->back = 0;
	q->population = 0;
	for(int i = 0; i <  QUEUE_SIZE; i++){
		q->strings[i] = NULL;
	}
}

void enqueue(char *element, Queue *q){
	q->strings[q->back] = element;
	q->back = (q->back + 1)  % QUEUE_SIZE;
	q->population++;
}

char *dequeue(Queue *q){
	char *item = q->strings[q->front];
	q->front = (q->front + 1) % QUEUE_SIZE;
	q->population--;
	return item;
}


int isDonorToRecipient(char *donor, char *recipient)
{
  if (strcmp(donor, "O") == 0 || strcmp(donor, recipient) == 0)
    return 1;
  if (strcmp(donor, "A") == 0 && strcmp(recipient, "AB") == 0)
    return 1;
  if (strcmp(donor, "B") == 0 && strcmp(recipient, "AB") == 0)
    return 1;
  return 0;
}


void printQueue(struct queue *q)
{
  printf("Printing queue %p\n", q);
  printf("\tThe index for the front of the queue is %d\n", q->front);
  printf("\tThe index for the back of the queue is %d\n", q->back);
  if (q->population == 0)
  {
    printf("\tThe queue is empty.\n");
  }
  else
  {
    for (int i = 0; i < q->population; i++)
    {
      int index = (q->front + i) % QUEUE_SIZE;
      printf("\t\tEntry[%d]: \"%s\"\n", index, q->strings[index]);
    }
  }
}


void prettyPrintQueue(struct queue *q, char *label, char *type)
{
  if (q->population == 0)
  {
    printf("No unmatched entries for %s (%s)\n", label, type);
  }
  else
  {
    printf("Unmatched %s (%s):\n", label, type);
    for (int i = 0; i < q->population; i++)
    {
      int index = (q->front + i) % QUEUE_SIZE;
      printf("%s\n", q->strings[index]);
    }
  }
}

char *stringCombination(char *str1, char *str2){
	char *p = str1 + strlen(str1);
	while(*str2 != '\0'){
		*p++ = *str2++;
	}
	*p = '\0';
	return str1;
}


int main(int argc, char **argv)
{
  char *types[BLOOD_TYPES] = {"AB", "B", "A", "O"};

  struct queue *donors[BLOOD_TYPES];
  struct queue *recipients[BLOOD_TYPES];
  struct queue *surgeons = malloc(sizeof *surgeons);

  for(int i = 0; i < BLOOD_TYPES; i++){
	  donors[i] = malloc(sizeof(struct queue));
	  recipients[i] = malloc(sizeof(struct queue));
	  initializeQueue(donors[i]);
	  initializeQueue(recipients[i]);
  }
  initializeQueue(surgeons);
 

  FILE *f = fopen(argv[1], "r");
  if(f == NULL){
	  exit(EXIT_FAILURE);
  }
  size_t size = LINE_LENGTH;
  char *buffer = malloc(size*sizeof(char));
  char blo[2];
  char type[3];
  char na[15];
  char last[15];
  while(getline(&buffer, &size, f) > 0){
	  if(buffer[0] == 'S'){
	  	sscanf(buffer, "%[^:]:%s %s", type, na, last);
		char *kind = strdup(type);
		int length = strlen(na);
		na[length] = ' ';
		na[length + 1] = '\0';
		stringCombination(na, last);
		char *name = strdup(na);
		int found = 0;
		if(*kind == 'S'){
			for(int j = 0; j < BLOOD_TYPES; j++){
				for(int r = 0; r < BLOOD_TYPES; r++){
					if(donors[j]->population > 0 && recipients[r]->population > 0){
						if(isDonorToRecipient(types[j], types[r]) == 1){
							printf("MATCH: %s donates to %s via Dr. %s\n", dequeue(donors[j]), dequeue(recipients[r]), name);
							found++;
							break;
						}
					}
				}
				if(found == 1){
					break;
				}
			}
			if(found == 0){
				enqueue(name, surgeons);
			}	

		}

			
		
	  } else {
		sscanf(buffer, "%[^:]:%[^:]:%s %s", type, blo, na, last);
		char *kind = strdup(type);
		char *blood = strdup(blo);
		int length2 = strlen(na);
		na[length2] = ' ';
		na[length2 + 1] = '\0';
		stringCombination(na, last);
		char *name = strdup(na);
		int num = 0;
		int found = 0;
		while(strcmp(blood, types[num]) != 0){
			num++;
		}
		if(*kind  == 'R'){
				for(int i = 0; i < BLOOD_TYPES; i++){
					if(donors[i]->population > 0 && surgeons->population > 0){
						if(isDonorToRecipient(types[i], blood)== 1){
							found = 1;
							printf("MATCH: %s donates to %s via Dr. %s\n", dequeue(donors[i]), name, dequeue(surgeons));
							break;
						}
					}
				}
				if(found != 1){
					enqueue(name, recipients[num]);
				}
				
		}  else if(*kind  == 'D'){
			for(int i = 0; i < BLOOD_TYPES; i++){
				if(recipients[i]->population > 0 && surgeons->population > 0){
					if(isDonorToRecipient(blood, types[i]) == 1){
						found = 1;
						printf("MATCH: %s donates to %s via Dr. %s\n", name, dequeue(recipients[i]), dequeue(surgeons));
						break;
					}
				}
			}
			if(found != 1){
				enqueue(name, donors[num]);
			}

		}

	}

		
}
  
 //print results
  for (int r = 0; r < BLOOD_TYPES; r++)
    prettyPrintQueue(recipients[r], "recipients", types[r]);
  for (int d = 0; d < BLOOD_TYPES; d++)
    prettyPrintQueue(donors[d], "donors", types[d]);
  prettyPrintQueue(surgeons, "surgeons", "type-agnostic");
  
  //free all memory allocated
  free(surgeons);
  for(int z = 0; z < BLOOD_TYPES;z++){
	  free(donors[z]);
	  free(recipients[z]);
  }

  return EXIT_SUCCESS;
}
