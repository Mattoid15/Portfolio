/******************************************************************/
/*   Author:           Matthew Lingenfelter                       */
/*   Creation Date:    November 1, 2021                           */
/*   Purpose:          understand TCP and UDP sockets, and        */
/*       properly implement them to create a client program       */
/*   Language:         C (version 8.2)                            */
/*   Compilation:      gcc qotd_MatthewLingenfelter.c             */
/*   Execution:        ./a.out <connection_type> <QOTD_Server>    */
/******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAXLENGTH 512

int main(int argc, char *argv[])
{ 
  char connection[4];
  char connectionLower[4];

  //checks if there are two command line arguments
  if( argc == 3 ) {
    strcpy(connection, argv[1]);
    
    size_t len = strlen(connection);
    
    //takes first command line argument and converts it to lowercase
    for(int i = 0; i < len; i++){
      connectionLower[i] = tolower(connection[i]);
    } //end converting to lowercase for
    
    //checks if the user choose either tcp or udp, if not, closes program
    if(strcmp(connectionLower, "tcp") != 0 && strcmp(connectionLower, "udp") != 0) {
      printf("Usage: %s connection_type\n", argv[0]);
      exit(1);
    } //end tcp or udp check if 
  } //end CLA check if
  
  else {
    printf("Usage: %s number_of_CLA\n", argv[0]);
    exit(1);
  } //end CLA check else
  

  int n, sockfd, sfd;
  struct addrinfo *serveraddrinfo = (struct addrinfo *) malloc(sizeof(struct addrinfo));
  struct sockaddr_in *servaddr;
  char *message[MAXLENGTH];

  //sets serveraddrinfo hint parameters
  bzero(serveraddrinfo, sizeof(serveraddrinfo));
  serveraddrinfo->ai_family = AF_INET;
  serveraddrinfo->ai_flags = 0;
  serveraddrinfo->ai_protocol = 0;

  
  //determins whether to use tcp or udp
  if(strcmp(connectionLower, "tcp") == 0) { //using tcp
    //sets socket type to SOCK_STREAM to use TCP
    serveraddrinfo->ai_socktype = SOCK_STREAM;
    
    //creates a tcp socket, and checks for errors
    sockfd = socket(serveraddrinfo->ai_family, serveraddrinfo->ai_socktype, 0);
    if(sockfd == -1) {
      perror("Error creating socket: ");
      exit(-1);
    } //end error check if
    
    printf("Socket Created\n");
    
    //gets IPv4 address from second CLA and checks if there was an error getting the info
    n = getaddrinfo(argv[2], NULL, serveraddrinfo, &serveraddrinfo);
    if(n != 0) {
      printf("Error in getaddrinfo call: %s\n", gai_strerror(n));
      exit(-1);
    } //end getaddrinfo error check if
    
    //sets address of the server
    servaddr = (struct sockaddr_in *)serveraddrinfo->ai_addr;
    
    //assigns port number
    servaddr->sin_port = htons(1024);
    
    //connects to server, then checks if there was an error connecting
    n = connect(sockfd, (struct sockaddr *)servaddr, sizeof(struct sockaddr));
    if(n == -1) {
      perror("Error connecting to server");
      exit(-1);
    } //end connect error check if
    
    printf("Connected\n");
    
    //Receive a reply from the server
    if(recv(sockfd, message, MAXLENGTH, 0) < 0) {
      printf("recv failed");
      exit(-1);
    } //ends receive reply error check if
    
    printf("%s\n", message);
    
  } //end using tcp if
  
  else { //using udp
    //sets socket type to SOCK_DGRAM to use UDP
    serveraddrinfo->ai_socktype = SOCK_DGRAM;
  
    //creates a udp socket, and checks for errors
    sockfd = socket(serveraddrinfo->ai_family, serveraddrinfo->ai_socktype, 0);
    if(sockfd == -1) {
      perror("Error creating socket: ");
      exit(-1);
    } //end error check if
    
    printf("Socket Created\n");
    
    //gets IPv4 address from second CLA and checks if there was an error getting the info
    n = getaddrinfo(argv[2], NULL, serveraddrinfo, &serveraddrinfo);
    if(n < 0) {
      printf("Error in getaddrinfo call: %s\n", gai_strerror(n));
      exit(-1);
    } //end getaddrinfo error check if
    
    //sets address of the server
    servaddr = (struct sockaddr_in *)serveraddrinfo->ai_addr;
    
    //assigns port number
    servaddr->sin_port = htons(17);
    
    //receive message from server
    if(recv(sockfd, message, MAXLENGTH, 0) < 0) {
      printf("recv failed");
      exit(-1);
    } //ends receive reply error check if
    
    printf("%s\n", message);
    
  } //end using udp else
} //end main
