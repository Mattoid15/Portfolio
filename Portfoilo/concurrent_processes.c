/************************************************************/
/*   Author:           Matthew Lingenfelter                 */
/*   Creation Date:    September 16, 2021                   */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//declaring global variable x
int x;

void childProccess(int, int*, int, int);

int main(int argc, char *argv[])
{
  int numProgs;
  
  //checks if there are only two command line arguments
  if( argc == 2 ) {
    numProgs = atoi(argv[1]);
    
    if(numProgs < 1 || numProgs > 10) {
      printf("Input must be between 1 and 10.\n");
      printf("Usage: %s number_of_processes\n", argv[0]);
      exit(1);
    } //end if  
  } //end if
  else {
    printf("Usage: %s number_of_processes\n", argv[0]);
    exit(1);
  } //end else
  
  x = 10;
  int num = 50;
  int *nump = malloc(sizeof(int));
  *nump = 100;
  
  pid_t npid;
  
  //starts creating sequential processes
  printf("\nCreating %d SEQUENCITAL processes\n", numProgs);
  printf("---------------------------------\n");
  printf("The current value of numProgs is %d. The current value of num is %d.", numProgs, num);
  printf("\nThe current value of nump is %d. The current value of x is %d.\n\n", *nump, x);
  
  //loops numProgs times and creates numProgs child sequential processes
  for(int i = 0; i < numProgs; i++) {
    npid = fork();
    
    //checks if there was an error during the fork call
    if (npid < 0) {
      perror("There was an error during fork call: ");
      exit(1);
    }  // end if
    
    if(npid == 0) {
      //child process function call
      childProccess(num, nump, x, i);
    } //end if
    npid = wait(NULL);
    
    //checks if the child process exited correctly
    if(npid == -1) {
      perror("There was an error exiting the child process:");
      exit(1);
    }
    printf("Child with PID of %i has ended.\n", npid);
  } //end for
  
  //resets variables
  x = 10;
    num = 50;
    *nump = 100;
    
    //starts creating concurrent processes
    printf("\nCreating %d CONCURRENT prosesses\n", numProgs);
    printf("--------------------------------\n");
	printf("The current value of numProgs is %d. The current value of num is %d.", numProgs, num);
  printf("\nThe current value of nump is %d. The current value of x is %d.\n\n", *nump, x);
    
    //loops numProgs times and creates numProgs child concurrent processes
    for(int i = 0; i < numProgs; i++) {
      npid = fork();
      
      //checks if there was an error during the fork call
      if (npid < 0) {
	perror("There was an error during fork call: ");
	exit(1);
      }  // end if
      
      if(npid == 0) {
	//calls the childProccess function
        childProccess(num, nump, x, i);
      } //end if
    } //end for
    
    //loops numProgs times and waits for each child to finish
    for(int i = 0; i < numProgs; i++) { //this is the code for the parent process
      npid = wait(NULL);
      
      //checks if the child process exited correctly
      if(npid == -1) {
	perror("There was an error exiting the child process:");
	exit(1);
      }
      printf("Child with PID of %i has ended.\n", npid);
    } //end for
    
    //final print message stating that all processes are done
    printf("\nAll processes have completed.\n");
}

/*************************************************************************/
/*                                                                       */
/*   Function name:     childProcess                                     */
/*   Description:       All processes the child must complete            */
/*   Parameters:        int num: the value of num - input                */
/*                      int *nump: the pointer nump - input              */
/*                      int x: the value of x – input                    */
/*                      int i: the value of i & the child number - intput*/
/*   Return Value:      none - void function                             */
/*                                                                       */
/*************************************************************************/
void childProccess(int num, int* nump, int x, int i) { //this is the code for the child process
  printf("BEFORE increment: Child number %d with PID of %d: num = %d, nump = %d, x = %d\n", i+1, getpid(), num, *nump, x);
  
  //increments the three variables
  x += 5;
  num += 1;
  (*nump)++;
  
  printf("AFTER increment:  Child number %d with PID of %d: num = %d, nump = %d, x = %d\n", i+1, getpid(), num, *nump, x);
  exit(1);
}
