/*
Report:

SOURCES: Hash functions from Sventek Book, collaborated with classmate

 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAX_SIZE 80000

typedef struct
{
    // Add data members here.
    char *keys[MAX_SIZE];
    float values[MAX_SIZE];
} ArrayDictionary;

void Initialize(ArrayDictionary *ad)
{
   // Implement me!
   for(int i = 0; i < MAX_SIZE; i++){
	   ad->keys[i] = NULL;
	   ad->values[i] = 0.;
   }
  
}


#define A 31L


long hash1(char *key, long N){
	unsigned long sum  = 0L;
	char *p;

	for(p = key; *p != '\0'; p++){
		sum = A * sum + (unsigned long)(*p);
	}
	return (long)(sum % N);
	//Hash function from Sventek book
}

#define B 41L
long hash2(char *key, long N){
	unsigned long sum = 0L;
	char *p;

	for(p = key; *p != '\0'; p++){
		sum  = B * sum + (unsigned long)(*p);
	}
	return (long)(B - (sum  % B));
	//Hash function from Sventek book but modified.
}


void Store(ArrayDictionary *ad, char *key, float value)
{
   // Implement me!
   long r = hash1(key, MAX_SIZE);
   long g = hash2(key, MAX_SIZE);
   int num = 1;
   if(ad->keys[r] == NULL){
	   ad->keys[r] = key;
	   ad->values[r] = value;
	   return;
   } else{
   	int new_num = (r +num * g) % MAX_SIZE;
   	while(ad->keys[new_num] != NULL){
		   new_num = (r + num* g) % MAX_SIZE;
		   num++;
	   }
   	ad->keys[new_num] = key;
   	ad->values[new_num] = value;
   	return;
   	}
   exit(EXIT_FAILURE);
}

int my_strcmp(char *str1, char *str2){
	int i = 0, result = 0;
	while(str1[i] != '\0'){
		if(str1[i] != str2[i]){
			result = 1;
		}
		i++;
	}
	return result;
}


float Fetch(ArrayDictionary *ad, char *key)
{
   // Implement me!
   int idx = hash1(key, MAX_SIZE);
   int idx2 =  hash2(key, MAX_SIZE);
   int num = 1;
   if(my_strcmp(key, ad->keys[idx]) == 0){
	  return ad->values[idx];
   } else {
	int new_num = (idx + num * idx2) % MAX_SIZE;
   	while(my_strcmp(key, ad->keys[new_num]) != 0){
		   new_num = (idx + num* idx2) % MAX_SIZE;
		   num++;
	   }
	return ad->values[new_num];
   }
   
   
   return 0.;
}


int main()
{
    FILE *f = fopen("DJIA", "r");
    if (f == NULL)
    {
         fprintf(stderr, "Unable to open file \"DJIA\" ... did you copy it to the current directory?\n");
         exit(EXIT_FAILURE);
    }

    ArrayDictionary ad;
    Initialize(&ad);

    //
    // Step 1: parse the file and place contents in the Dictionary.
    //  The key should be the date (a char *)
    //  The value should be the closing price.  (look at DJIA for more info)
    //
    char d[10];
    float O, H, L, C ,sum = 0.0;
    size_t size = 1024;
    char *buffer = malloc(size*sizeof(char));

    struct timeval startTime;
    struct timeval endTime;
    gettimeofday(&startTime, 0);
    while (getline(&buffer, &size, f) > 0)
    {	
	sscanf(buffer, "%s %f %f %f %f", d, &O, &H, &L, &C);
        //printf("The line is \"%s\"\n", buffer);
        // Hint 1: sscanf is great.  Turn that line into numbers!
        // Hint 2: be very careful about what you send in to Store.
        //         The memory at buffer will be changing, so you can't
        //         use that.  Big hint: strdup
	char *str = strdup(d);
	float closePrice = C;
	if(str[0] <  57  && str[0] > 47){
        	Store(&ad, str, closePrice);
	}
    }
    gettimeofday(&endTime, 0);
    double time = (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.;
    printf("Time:%g\n", time);
    

    //
    //  For each data, do a fetch.
    //  For each returned closing price, add it to a running sum.
    //  Your goal is ultimately to get the average closing price over
    //  all of the dates in the file.
    //

    

    char dates[91][10] = {
                 "02/19/21", "02/19/20", "12/19/19", "12/12/19", "12/02/19", "11/22/19", "11/12/19", "10/22/19", "10/02/19", 
                 "09/12/19", "08/22/19", "08/12/19", "08/02/19", "07/22/19", "07/12/19", "07/02/19", "06/12/19", "05/22/19", 
                 "05/02/19", "04/22/19", "04/12/19", "04/02/19", "03/22/19", "03/12/19", "02/22/19", "02/19/19", "02/12/19", 
                 "01/22/19", "01/02/19", "12/19/18", "12/19/17", "12/19/16", "02/19/16", "02/19/15", "12/19/14", "02/19/14", 
                 "12/19/13", "02/19/13", "12/19/12", "12/19/11", "02/19/10", "02/19/09", "12/19/08", "02/19/08", "12/19/07", 
                 "12/19/06", "12/19/05", "02/19/04", "12/19/03", "02/19/03", "12/19/02", "02/19/02", "12/19/01", "12/19/00", 
                 "02/19/99", "02/19/98", "12/19/97", "02/19/97", "12/19/96", "12/19/95", "12/19/94", "02/19/93", "02/19/92", 
                 "12/19/91", "02/19/91", "12/19/90", "12/19/89", "12/19/88", "02/19/88", "02/19/87", "12/19/86", "02/19/86", 
                 "12/19/85", "02/19/85", "12/19/84", "12/19/83", "02/19/82", "02/19/81", "12/19/80", "02/19/80", "12/19/79", 
                 "12/19/78", "12/19/77", "02/19/76", "12/19/75", "02/19/75", "12/19/74", "02/19/74", "12/19/73", "12/19/72", 
                 "02/19/71"
           };

    gettimeofday(&startTime, 0); 
    for (int i = 0 ; i < 91 ; i++)
    {
        // You will need to repeat the code from the last while loop to get the data as a "char *"
        float val = Fetch(&ad, dates[i]);
        if (val == 0.)
        {
            printf("Bad fetch!\n");
            exit(EXIT_FAILURE);
        }
	sum = sum + val;
    }
    gettimeofday(&endTime, 0);
    double time2 = (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.;
    printf("Time 2:%g\n", time2);
    fclose(f);

    
    
    // Uncomment this.  Not it assumes you called your sum variable "sum"
    printf("Over the 91 days, the average value was %d\n", (int)(sum/91.0));
    // It should print:
    //    Over the 91 days, the average value was 13209
    //
    
}
