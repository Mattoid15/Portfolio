/******************************************************************/
/*   Author:           Matthew Lingenfelter                       */
/*   Creation Date:    November 12, 2021                          */
/*   Purpose:          This is the server for the RPS game.       */
/*   Language:         C (version 8.2)                            */
/*   Compilation:      make server                                */
/*                        gcc -c libraries.c                      */
/*                        gcc -c server.c                         */
/*                        gcc -o server server.o libraries.o      */
/*   Execution:        ./server <number_of_rounds> <port_number>  */
/******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "libraries.h"

#define DefaultPort 1080
#define Read 0
#define Write 1
#define Nick 99

void initialize(char* [], int*, int*);

int main(int argc, char* argv[]) {
  int rounds, round_number, connections, sockfd, len, connfd1, connfd2, cli_num;
  int serv_cli1[2], cli1_serv[2], serv_cli2[2], cli2_serv[2];
  int port_number = DefaultPort;
  struct sockaddr_in servaddr, cli;
  char *yesNo = malloc(sizeof(char));
  int *yInt = malloc(sizeof(int));
  pid_t npid;
  round_number, connections, cli_num = 0;
  *yInt = 1;
  
  //get and check the number of rounds to be played and what port number to use
  if(argc == 2) { //port number not provided
    argv[2] = (char*) &port_number;
    initialize(argv, &port_number, &rounds);
  }
  else if(argc == 3) { //port number provided
    initialize(argv, &port_number, &rounds);
  }
  else { //invalid number of CLAs
    printf("Usage: %s <number_of_rounds> <port_number>\n", argv[0]);
    exit(-1);
  }
  
  //create socket and check that it was succeful
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1) {
    perror("Socket Creation Failed");
    exit(-1);
  }
  
  printf("Socket successfully created.\n");
  bzero(&servaddr, sizeof(servaddr));
  
  //assigns IP address and port number
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port_number);
  
  //binds socket created to given IP and checks if it was successful
  if((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
    perror("Socket bind failed");
    exit(-1);
  }
  
  printf("Socket successfully binded.\n\n");
  
  //daemon process--------------------------------------------------
  //loops until told to stop
  while(*yInt != 0) {
    //check for errors creating the pipe
    if(pipe(serv_cli1) == -1) {
      perror("Error while creating serv_cli1 pipe");
      exit(-1);
    }
    if(pipe(cli1_serv) == -1) {
      perror("Error while creating cli1_serv pipe");
      exit(-1);
    }
    if(pipe(serv_cli2) == -1) {
      perror("Error while creating serv_cli2 pipe");
      exit(-1);
    }
    if(pipe(cli2_serv) == -1) {
      perror("Error while creating cli2_serv pipe");
      exit(-1);
    }
    
    //loops to connect to two clients
    while(connections != 2) {
      //sets the socket to listen for incoming connections and checks if it was succeful
      //only allows 2 clients to be in listen queue 
      if(listen(sockfd, 2) != 0) {
        perror("Listen failed");
        exit(-1);
      }
    
      printf("Server listening..\n");
      len = sizeof(cli);
    
      //accepts the connection to the client and checks if it was successful
      //blocks the rest of the program until a connection is established
      if(connections == 0) {
        connfd1 = accept(sockfd, (struct sockaddr*)&cli, &len);
        if(connfd1 == -1) {
          perror("Server accept failed");
          exit(-1);
        }
      }
      else if(connections == 1) {
        connfd2 = accept(sockfd, (struct sockaddr*)&cli, &len);
        if(connfd2 == -1) {
        perror("Server accept failed");
        exit(-1);
        }
      }
    
      connections++;
      printf("Client %d accepted\n", connections);
    
      //creates child process for each client to connect
      npid = fork();
      cli_num++;
      
      //checks if there was an error during the fork call
      if(npid < 0) {
        perror("There was an error during fork call: ");
        exit(-1);
      }
      else if(npid == 0) {
        //child breaks out of the loop to handle communication to client
        break;
      }
    }
    
    //PARENT CODE-----------------------------------------------------
    if(npid != 0) {
      //close reading end of the pipes
      close(serv_cli1[Read]);
      close(serv_cli2[Read]);
      //close writing end of the pipes
      close(cli1_serv[Write]);
      close(cli2_serv[Write]);
      
      int *choice1 = malloc(sizeof(int));
      int *choice2 = malloc(sizeof(int));
      int *myscore1 = malloc(sizeof(int));
      int *myscore2 = malloc(sizeof(int));
      char *nick1 = malloc(sizeof(char[Nick]));
      char *nick2 = malloc(sizeof(char[Nick]));
      char *msg = malloc(sizeof(char[5]));
      char winner[Nick] = {'t', 'i', 'e'};
      
      *myscore1 = 0;
      *myscore2 = 0;
      
      //loops until both nicknames are unique
      while(1 == 1) {
        //gets player1's nickname
        readMsg(cli1_serv[Read], msg, 4);
        if (strcmp(msg, "NICK") == 0) {
          readMsg(cli1_serv[Read], nick1, Nick);
        }
        
        //gets player2's nickname
        readMsg(cli2_serv[Read], msg, 4);
        if (strcmp(msg, "NICK") == 0) {
          readMsg(cli2_serv[Read], nick2, Nick);
        }
        
        //exits the loop if the 2 nicknames are not the same
        if (strcmp(nick1, nick2) != 0) {
          writeMsg(serv_cli1[Write], "READY", 5);
          writeMsg(serv_cli2[Write], "READY", 5);
          break;
        }
        
        //tells the clients to send a new nickname
        writeMsg(serv_cli1[Write], "RETRY", 5);
        writeMsg(serv_cli2[Write], "RETRY", 5);
      }
      
      printf("\nPlayer1's nickname is %s\n", nick1);
      printf("Player2's nickname is %s\n\n", nick2);
      
      //loops for the number of rounds provided
      while(rounds != 0) {
        round_number++;
        printf("Round %d\n", round_number);
        
        //gets player1's choice for the round
        readInt(cli1_serv[Read], choice1);
        arrayName(1, *choice1);
        
        //gets player2's choice for the round
        readInt(cli2_serv[Read], choice2);
        arrayName(2, *choice2);
        
        //determins who wins the round and updates the scores
        tictactoe(*choice1, *choice2, myscore1, myscore2);
        printf("%s: %d  %s: %d\n\n", nick1, *myscore1, nick2, *myscore2);
        
        rounds--;
      }
      
      //decides who the winner is and tell both clients
      if(*myscore1 > *myscore2) { //player 1 wins
        printf("The winner is %s.\n\n", nick1);
        writeInt(serv_cli1[Write], myscore2); //sends losing score
        writeInt(serv_cli1[Write], myscore1); //sends winning score
        writeMsg(serv_cli1[Write], nick1, Nick); //sends winners name
        writeInt(serv_cli2[Write], myscore2);
        writeInt(serv_cli2[Write], myscore1);
        writeMsg(serv_cli2[Write], nick1, Nick);
      }
      else if(*myscore2 > *myscore1) { //player 2 wins
        printf("The winner is %s.\n\n", nick2);
        writeInt(serv_cli1[Write], myscore1);
        writeInt(serv_cli1[Write], myscore2);
        writeMsg(serv_cli1[Write], nick2, Nick);
        writeInt(serv_cli2[Write], myscore1);
        writeInt(serv_cli2[Write], myscore2);
        writeMsg(serv_cli2[Write], nick2, Nick);
      }
      else { //is was a tie
        printf("It was a tie.\n\n");
        writeInt(serv_cli1[Write], myscore1);
        writeInt(serv_cli1[Write], myscore2);
        writeMsg(serv_cli1[Write], "Tie", Nick);
        writeInt(serv_cli2[Write], myscore1);
        writeInt(serv_cli2[Write], myscore2);
        writeMsg(serv_cli2[Write], "Tie", Nick);
      }
      
      //frees allocated memory
      free(choice1);
      free(choice2);
      free(myscore1);
      free(myscore2);
      free(nick1);
      free(nick2);
      free(msg);
      
      //closes the pipes
      close(serv_cli1[Write]);
      close(serv_cli2[Write]);
      close(cli1_serv[Read]);
      close(cli2_serv[Read]);
    }
    //END PARENT CODE-------------------------------------------------
    
    //CHILD CODE------------------------------------------------------
    if(npid == 0) {
      int *score = malloc(sizeof(int));
      int *winner_score = malloc(sizeof(int));
      int *choice = malloc(sizeof(int));
      int connfd;
      char *nick = malloc(sizeof(char[Nick]));
      char winner[Nick];
      char *message = malloc(sizeof(char[20]));
      
      //closes the pipes determined by which child process this is (1 or 2)
      if(cli_num == 1) {
        connfd = connfd1;
        close(serv_cli1[Write]);
        close(cli1_serv[Read]);
        close(serv_cli2[Read]);
        close(serv_cli2[Write]);
        close(cli2_serv[Read]);
        close(cli2_serv[Write]);
      }
      else if(cli_num == 2) {
        connfd = connfd2;
        close(serv_cli2[Write]);
        close(cli2_serv[Read]);
        close(serv_cli1[Read]);
        close(serv_cli1[Write]);
        close(cli1_serv[Read]);
        close(cli1_serv[Write]);
      }
      
      //gets READY string from client
      readMsg(connfd, message, 5);
      
      //loops until both nicknames are unique
      while(1 == 1) {
        //get NICK string from client
        readMsg(connfd, nick, Nick);
        
        //send NICK to parent to associate each process with the nickname
        if(cli_num == 1) {
          writeMsg(cli1_serv[Write], nick, Nick);
          readMsg(serv_cli1[Read], message, 5);
        }
        else if(cli_num == 2) {
          writeMsg(cli2_serv[Write], nick, Nick);
          readMsg(serv_cli2[Read], message, 5);
        }
        
        //exits the loop if the sever did not send RETRY
        if(strcmp(message, "RETRY") != 0)
          break;
        
        //if it did, send RETRY to client
        writeMsg(connfd, "RETRY", 5);
      }
      
      //send READY to client to confirm nickname is valid
      writeMsg(connfd, "READY", 5);
      
      //loops for the number of rounds to be played
      while(rounds != 0) {
        //send GO to the client to start the round
        writeMsg(connfd, "GO", 2);
        
        //get client choice for the round
        readInt(connfd, choice);
        
        //send client choice to parent
        if(cli_num == 1) {
          writeInt(cli1_serv[Write], choice);
        }
        else if(cli_num == 2) {
          writeInt(cli2_serv[Write], choice);
        }
        rounds--;
      }
      
      //reads the player's score and who the winner is
      if(cli_num == 1) {
        readInt(serv_cli1[Read], score);
        readInt(serv_cli1[Read], winner_score);
        readMsg(serv_cli1[Read], winner, Nick);
      }
      else if(cli_num == 2) {
        readInt(serv_cli2[Read], score);
        readInt(serv_cli2[Read], winner_score);
        readMsg(serv_cli2[Read], winner, Nick);
      }
      
      //send SCORE and the winner to the client
      writeMsg(connfd, "SCORE", 5);
      writeInt(connfd, score);
      writeInt(connfd, winner_score);
      writeMsg(connfd, winner, Nick);
      
      //send STOP to end the connection
      writeMsg(connfd, "STOP", 4);
      
      //closes the pipes
      if(cli_num == 1) {
        close(serv_cli1[Read]);
        close(cli1_serv[Write]);
      }
      else if(cli_num == 2) {
        close(serv_cli2[Read]);
        close(cli2_serv[Write]);
      }
      
      //closes the connections
      close(connfd);
      close(connfd1);
      close(connfd2);
      
      //frees allocated memory
      free(score);
      free(winner_score);
      free(choice);
      free(message);
      
      bzero(&connfd, sizeof(connfd));
      
      //exits the child program
      exit(1);
    }
    //END CHILD CODE--------------------------------------------------
    
    //waits for each child process to end
    //child processes should end after the connection closes
    for(int i = 0; i < 2; i++) {
      npid = wait(NULL);
      //checks if the child process exited correctly
      if(npid == -1) {
        perror("There was an error exiting the child process:");
        exit(-1);
      }
    }
    
    //prompt user to continue server program or not
    printf("End the server? - yes(y), no(n):\n");
    fgets(yesNo, 3, stdin);
    yesNo[strcspn(yesNo, " ")] = '\0';
    if(*yesNo == 'y') {*yInt = 0;}
    else if(*yesNo == 'n') {*yInt = 1;}
    else {*yInt = 0;}
    
    //ends the connections
    close(connfd1);
    close(connfd2);
    
    //resets game variables
    connections = 0;
    cli_num = 0;
    round_number = 0;
    rounds = atoi(argv[1]);
    
  } //--------------------------------------------------------------
  
  //closes the pipes
  close(serv_cli1[Write]);
  close(serv_cli2[Write]);
  close(cli1_serv[Read]);
  close(cli2_serv[Read]);
  
  //closes the socket
  close(sockfd);
  
  free(yesNo);
  free(yInt);
}

/******************************************************************/
/*                                                                */
/*   Function Name: initialize                                    */
/*   Description:   assigns the number of rounds to be played and */
/*                  the port number to be used by the program     */
/*   Parameters:    argv[] - all the command line arguments       */
/*                  *port - pointer to the port_number variable   */
/*                  *rounds - poinrt to the rounds variable       */
/*   Return Value:  none - void function                          */
/*                                                                */
/******************************************************************/
void initialize(char* argv[], int* port, int* rounds) {
  //sets the rounds to be played and the port number to be used
  *rounds = atoi(argv[1]);
  *port = atoi(argv[2]);
  //checks if the port number was given, if not use the default port number
  if(*port == 0)
    *port = DefaultPort;
  printf("%d rounds will be played.\n", *rounds);
  printf("Port number %d will be used.\n", *port);
}
