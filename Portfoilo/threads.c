/******************************************************************/
/*   Author:           Matthew Lingenfelter                       */
/*   Creation Date:    September 24, 2021                         */
/*   Purpose:          The purpose of this program is to          */
/*                     understand threads and shared memory       */
/*                     differences between processes and threads  */
/*   Language:         C (version 8.2)                            */
/*   Compilation:      gcc threads_MatthewLingenfelter.c -pthread */
/*   Execution:        ./a.out <numThreads>                       */
/******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>	

//creates global variable x
int x;

//struct with variables for each thread
struct Values {
  int num;
  int *nump;
  int threadNum;
};

//function that each thread will complete
void *threadProccess (void *arg);

int main(int argc, char *argv[]){
  int numThreads;
  
  //checks if there are only two command line arguments
  if( argc == 2 ) {
    numThreads = atoi(argv[1]);
    
    //checks if numThreads is between 1 and 10
    if(numThreads < 1 || numThreads > 10) {
      printf("Input must be between 1 and 10.\n");
      printf("Usage: %s number_of_processes\n", argv[0]);
      exit(1);
    } //end if  
  } //end if
  else {
    printf("Usage: %s number_of_processes\n", argv[0]);
    exit(1);
  } //end else
  
  //inisialize each variable to the correct value
  x = 10;
  int num = 50;
  int *nump = malloc(sizeof(int));
  *nump = 100;
  
  //creates thread_id array of size numThreads
  pthread_t thread_id[numThreads];
  
  //creates an array of struct pointers, one for each thread
  struct Values *threads[numThreads];
  
  //starts creating threads
  printf("\nCreating %d Threads\n", numThreads);
  printf("-------------------\n");
  printf("The current value of numThreads is %d. The current value of num is %d.", numThreads, num);
  printf("\nThe current value of nump is %d. The current value of x is %d.\n\n", *nump, x);
  
  int rc;
  
  //loops numThreads times and creates numThreads threads
  for (int i = 0; i < numThreads; i++) {
    //allocates memory for this threads struct
    threads[i] = ((struct Values*) malloc(sizeof(struct Values)));
    
    //assigns each variable to the appropriate value in the struct
    threads[i]->num = num;
    threads[i]->nump = malloc(sizeof(int));
    *threads[i]->nump = *nump;
    threads[i]->threadNum = i+1;
    
    //creates the thread
    rc = pthread_create(&thread_id[i], NULL, threadProccess, threads[i]);
    
    //checks if there was an error creating the thread
    if(rc != 0){
      strerror(rc);
      exit(1);
    } //end if
  } //end for loop
  
  //loops numThreads times and joins the thread that ended
  for (int i = 0; i < numThreads; i++) {
    rc = pthread_join(thread_id[i], NULL);
    
    //checks if ther was an error joining threads
    if(rc != 0){
      strerror(rc);
      exit(1);
    } //end if
  } //end for loop
	
	//loops numThreads times and frees memory of the array of struct pointers
	for (int i = 0; i < numThreads; i++) {
		free(threads[i]);
	}
	free(nump);
  
  //ends main thread
  pthread_exit(NULL);
} //end main function

/*************************************************************************/
/*                                                                       */
/* Function name:   threadProccess                                       */
/* Description:     All proccesses each thread must complete             */
/* Parameters:      void *arg - stuct assigned to the particular thread  */
/* Return Value:    none - void function                                 */
/*                                                                       */
/*************************************************************************/
void *threadProccess (void *arg) {
  struct Values *Value = (struct Values*)arg;
  struct Values threadStruct = *Value;
  
  printf("BEFORE increment: Thread number %d with TID of %d: num = %d, nump = %d, x = %d\n", threadStruct.threadNum, pthread_self(), threadStruct.num, *threadStruct.nump, x);
  
  //increments the three variables
  x += 5;
  threadStruct.num++;
  (*threadStruct.nump)++;
  
  printf("AFTER increment:  Thread number %d with TID of %d: num = %d, nump = %d, x = %d\n\n", threadStruct.threadNum, pthread_self(), threadStruct.num, *threadStruct.nump, x);
  
  //frees the memory address for this thread's nump pointer
  free(threadStruct.nump);
  
  //exits the current thread
  pthread_exit(NULL);
} //end threadProcess frunction