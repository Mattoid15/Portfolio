/****************************************************************/
/*   Author:           Travis Bubb                              */
/*   Creation Date:    November 21, 2021                        */
/*   Purpose:          This is the client for the RPS game.     */
/*   Language:         C (version 8.2)                          */
/*   Compilation:      gcc -o client client.c                   */
/*   Execution:        ./client <server_hostname> <port_number> */
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include "libraries.h"

/***************************************************************************/
/*   Name:          main                                                   */
/*   Description:   The main function for the client; Will connect to the  */
/*                  hostname at the given port and player rock, paper,     */
/*                  scissors with another instance of the program.         */
/*   Parameters:    int argc - an integer value representing the number    */
/*                             of command-line arguments that were passed. */
/*                  char **argv - a list of strings where each string is   */
/*                                a command-line argument.                 */
/*   Return:        None                                                   */
/***************************************************************************/
int main(int argc, char **argv) 
{
    // Declare variables
    char *nickname = malloc(sizeof(char[99]));
    char *hostname;
    char *wMsg = malloc(sizeof(char[64]));
    char *rMsg = malloc(sizeof(char[64]));
    char *choice = malloc(sizeof(char));
    int *wInt = malloc(sizeof(int));
    int *rInt = malloc(sizeof(int));
    int *nickSize = malloc(sizeof(int));
    int portNumber;
    int sockfd, ret;
    struct sockaddr_in *servaddr;
    struct addrinfo *hintsinfo = (struct addrinfo *)malloc(sizeof(struct addrinfo));
    struct addrinfo *servaddrinfo = (struct addrinfo *)malloc(sizeof(struct addrinfo));

    /* Process the command-line arguments; set the server hostname and portnumber */
    if (argc == 2) 
    { // A hostname is given but not a port number
        hostname = argv[1];
        portNumber = 1080;  // Default port number = 1080
    } 
    else if (argc == 3) 
    { // A hostname and port number are given
        hostname = argv[1];
        portNumber = atoi(argv[2]);
    } 
    else 
    { // An incorrect number of CLAs are given
        printf("Usage: %s server_hostname <port_number>\n", argv[0]);
        exit(1);
    }

    // Fill out values in hintsinfo to be used for getaddrinfo call   
    // Ensures that the specific address type needed will be returned 
    hintsinfo->ai_family = AF_INET;
    hintsinfo->ai_flags = 0;
    hintsinfo->ai_protocol = 0;
    hintsinfo->ai_socktype = SOCK_STREAM;

    // Get the server's info using getaddrinfo
    ret = getaddrinfo(hostname, NULL, hintsinfo, &servaddrinfo);
    if (ret < 0)
    {
        printf("Error in call to getaddrinfo: %s\n", gai_strerror(ret));
        exit(-1);
    }

    // Get the sockaddr_in struct for the server
    servaddr = (struct sockaddr_in *)servaddrinfo->ai_addr;
    servaddr->sin_port = htons(portNumber);

    // Create a TCP socket 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Error creating socket for client");
        exit(-1);
    }
    printf("Socket created\n");

    /* Make a connection to the server */
    ret = connect(sockfd, (struct sockaddr *) servaddr, sizeof(struct sockaddr));
    if (ret == -1)
    {
        perror("Error making connection to the server");
        exit(-1);
    }
    printf("Connection made\n");

    strcpy(wMsg, "READY");
    writeMsg(sockfd, wMsg, strlen(wMsg));
    printf("Write: %s\n", wMsg);

    while (strcmp(rMsg, "READY") != 0) 
    // Send the nickname to the server until READY message received
    {
        // Prompt the user to select a nickname
        printf("Please enter a nickname with no spaces to use:\n");
        fgets(nickname, 99, stdin);

        // Get rid of the trailing linefeed (caused by hitting enter for stdin)
        nickname[strcspn(nickname, "\n")] = '\0';
        nickname[strcspn(nickname, " ")] = '\0';

        if (strlen(nickname) < 1 || strlen(nickname) > 20)
        {
            printf("Please keep your nickname between 1 and 20 characters, inclusive!\n");
            continue;
        }

        *nickSize = strlen(nickname);

        // Send the nickname to the server
        writeMsg(sockfd, "NICK", 4);
        // writeInt(sockfd, nickSize);
        writeMsg(sockfd, nickname, *nickSize);

        // Receive either RETRY or READY from the server
        readMsg(sockfd, rMsg, 5);
    }   

    while (strcmp(rMsg, "SCORE") != 0)
    {
        // Reset the value of rMsg
        memset(rMsg, ' ', strlen(rMsg));
        memset(choice, ' ', strlen(choice));

        *wInt = -1;
        
        // Read a 5-byte message from the server
        readMsg(sockfd, rMsg, 5);

        // Get rid of any extra space characters in the string
        rMsg[strcspn(rMsg, " ")] = '\0';
 
        // printf("Read: %s\n", rMsg);

        if (strcmp(rMsg, "GO") == 0)
        {
            while (1 == 1)
            {
                // Prompt the user to select rock(r), paper(p), or scissors(s)
                printf("Please enter a move - rock(r), paper(p), or scissors(s):\n");
                fgets(choice, 3, stdin);
                choice[strcspn(choice, " ")] = '\0';
                // printf("client choice: %s\n", choice);
                if (*choice == 'r') {*wInt = 0;}
                else if (*choice == 'p') {*wInt = 1;}
                else if (*choice == 's') {*wInt = 2;}
                else {printf("Please provide a valid input.\n"); continue;}
                writeInt(sockfd, wInt);
                break;
            }
        }
    }

    int loser, winner = 0;
    // Recieve the loser score
    readInt(sockfd, rInt);
    loser = *rInt;

    // Read the winner's score
    readInt(sockfd, rInt);
    winner = *rInt;

    // Read the winner
    readMsg(sockfd, rMsg, 10);

    if (winner == loser)
    {
        printf("\nIt's a draw: %i-%i!\n", winner, loser);
    } 
    else
    {
        printf("\n%s is the winner with a %i - %i score!\n", rMsg, winner, loser);
    }


    // Terminate when received STOP from server
    readMsg(sockfd, rMsg, 4);
    if (strcmp(rMsg, "STOP") == 0)
    {
        // Close the socket
        close(sockfd);

        // Free all allocated memory
        free(wMsg);
        free(rMsg);
        free(choice);
        free(wInt);
        free(rInt);
        free(hintsinfo);
        free(servaddrinfo);
        free(nickSize);
        free(nickname);
        return 0;
    }
}
