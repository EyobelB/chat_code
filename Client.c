#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>

#define MAX_ARR_LEN 256



//ASSUME CLIENT CODE HERE
int main()
{
    //Create a socket
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    //Specify an address for the client
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(clientSocket, (struct sockaddr *) &server_address, sizeof(server_address));

    //Check for potential connection errors
    if (connection_status == -1)
    {
        printf("Server: There was an error in making a connection to the remote socket.");
        close(clientSocket);
    }

    //This code will receive(recv) data from a server
    char server_output[MAX_ARR_LEN];
    char* clientPasscode = calloc(MAX_ARR_LEN, sizeof(char));
    char pwdMessage[MAX_ARR_LEN];

    recv(clientSocket, &server_output, sizeof(server_output), 0);
    printf("Server: %s\n", server_output);
    recv(clientSocket, &server_output, MAX_ARR_LEN, 0);

    char pwdRequest[MAX_ARR_LEN] = "Enter a password below";

    //print out data that has been received from the server
    printf("Server: %s\n", pwdRequest);
    scanf("%s", clientPasscode);

    //Optimize the memory allocation by removing everything after the new line terminator
    int stringSize = 0;
    for(int i = 0; i < MAX_ARR_LEN; i++)
    {
        if(clientPasscode[i] != '\n')
        {
            continue;
        }
        else
        {
            clientPasscode = (char*) realloc(clientPasscode, i + 1);
        }
    }


    //Send the client passcode to the server
    send(clientSocket, clientPasscode, sizeof(clientPasscode), 0);

    //Allow for message to be received from server as to whether or not kick has happened
    recv(clientSocket, &pwdMessage, MAX_ARR_LEN, 0);
    printf("%s", pwdMessage);

    close(clientSocket);

    return 0;
}