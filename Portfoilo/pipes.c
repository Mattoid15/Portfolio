/******************************************************************/
/*   Author:           Matthew Lingenfelter                       */
/*   Creation Date:    October 22, 2021                           */
/*   Purpose:          The purpose of this program is to          */
/*                     understand pipes and correclty implement   */
/*                     them in a program                          */
/*   Language:         C (version 8.2)                            */
/*   Compilation:      gcc pipes_MatthewLingenfelter.c            */
/*   Execution:        ./a.out                                    */
/******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void errors(char*, int);

int main() {
  int p2c[2], c2p[2], n, npid;
    
  n = pipe(p2c);
  //check for errors creating the pipe
  if(n == -1)
    errors("Error while creating p2c pipe: ", 0);
  
  n = pipe(c2p);
  //check for errors creating the pipe
  if(n == -1)
    errors("Error while creating p2c pipe: ", 0);
  
  npid = fork();
  
  //checks if there was an error during the fork call
  if (npid < 0)
    errors("There was an error during the fork call: ", 0);
  
  if(npid == 0) {
    //child code--------------------------------------------------
    int *num = malloc(sizeof(int));
    int *newNum = malloc(sizeof(int));
    int *check = malloc(sizeof(int));
    int temp = (int)getpid();
    char msg[3];
    
    //reads random int from parent
    n = read(p2c[0], num, 4);
    printf("Child received from pipe: %d\n", *num);
    //checks for errors reading from pipe
    if(n == -1)
      errors("Error reading from pipe from child.", 0);
    
    //reads new int from parent
    n = read(p2c[0], newNum, 4);
    printf("Child received from pipe: %d\n", *newNum);
    //checks for errors reading from pipe
    if(n == -1)
      errors("Error reading from pipe from child.", 0);
    
    *check = *num * temp;
    
    //checks if the newNum is the correct value
    if(*check == *newNum) {
      //writes an approved message to the parent
      n = write(c2p[1], "Approved", 8);
      printf("Child sending to pipe from child: Approved\n");
      //checks for errors writing to pipe
      if(n == -1)
	errors("Error writing to pipe from child.", 0);
    }
    else {
      //writes a denied message to the parent
      n = write(c2p[1], "Denied", 6);
      printf("Child sending to pipe from child: Denied\n");
      //chekcs for errors writing to pipe
      if(n == -1)
	errors("Error writing to pipe from child.", 0);
    }
    
    //reading final message from parent
    n = read(p2c[0], msg, 3);
    printf("Child received from pipe: %s\n", msg);
    //checks for errors reading from pipe
    if(n == -1)
      errors("Error reading from pipe from child.", 0);
    
    //closes pipes
    close(p2c[0]);
    close(c2p[1]);
    
    //frees memory
    free(num);
    free(newNum);
    free(check);
    
    exit(1); //end child process
  } //end if
  
  //parent code---------------------------------------------------
  //close read end of p2c pipe
  close(p2c[0]);
  //close write end of c2p pipe
  close(c2p[1]);
  
  int *rnum = malloc(sizeof(int));
  char msg[8];
  
  //Creates a random number between 0 and 100
  srand(getpid());
  *rnum = (rand() % 101);
  
  //writes random number to pipe for child
  n = write(p2c[1], rnum, 4);
  printf("Parent sending to pipe: %d\n", *rnum);
  //checks if there was an error writing to pipe
  if(n == -1)
    errors("Error writing to pipe from parent.", 0);
  
  *rnum *= npid;
  
  //writes random number times child pid to pipe
  n = write(p2c[1], rnum, 4);
  printf("Parent sending to pipe: %d\n", *rnum);
  //checks if there was an error writing to pipe
  if(n == -1)
    errors("Error writing to pipe from parent.", 0);
  
  //reads message from child
  n = read(c2p[0], msg, 8);
  printf("Parent received from pipe: %s\n", msg);
  //checks if there was an error reading from pipe
  if(n == -1)
    errors("Error reading from pipe from parent.", 0);
  
  //writes final message to the child
  n = write(p2c[1], "BYE", 3);
  printf("Parent sending to pipe: BYE\n");
  //checks if there was an error writing to pipe
  if(n == -1)
    errors("Error writing to pipe from parent.", 0);
  
  npid = wait(NULL);
  //checks if the child process exited correctly
  if(npid == -1)
    errors("There was an error exiting the child process:", 0);
  
  //close pipes
  close(p2c[1]);
  close(c2p[0]);
  
  //frees memory
  free(rnum);
  
  return 0; //end program
}

/******************************************************************/
/*                                                                */
/*   Funcion name:   errors                                       */
/*   Description:    Handles error messages, outputs the proper   */
/*                 message with the error, and closes the program */
/*   Parameters:     char *errMsg - error message provided        */
/*                   int errNum - error number provided           */
/*   Return Value:   none                                         */
/*                                                                */
/******************************************************************/
void errors(char *errMsg, int errNum) {
  if(errNum == 0) //use perror
    perror(errMsg);
  else //use strerror
    printf("%s: %s\n", errMsg, strerror(errNum));
  exit(1);
}
