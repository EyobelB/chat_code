#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

#define MAX_ARR_LEN 256



//ASSUME CLIENT CODE HERE
int main()
{
    //Define time immediately
    time_t timeTrackerInSeconds, val = 1;

    //Pointer to other types of time
    struct tm* current_time;
    timeTrackerInSeconds = time(NULL);

    current_time = localtime(&timeTrackerInSeconds);



    //Create a socket
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    //Specify an address for the client
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6667);
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
    char* clientUsername = calloc(MAX_ARR_LEN, sizeof(char));
    char* clientPasscode = calloc(MAX_ARR_LEN, sizeof(char));
    char pwdMessage[MAX_ARR_LEN];

    char usernameRequest[MAX_ARR_LEN];
    char pwdRequest[MAX_ARR_LEN];

    recv(clientSocket, &server_output, sizeof(server_output), 0);
    printf("Server: %s\n", server_output);
    recv(clientSocket, &server_output, MAX_ARR_LEN, 0);



    //Start asking for username
    recv(clientSocket, &usernameRequest, MAX_ARR_LEN, 0);
    printf("Server: %s\n", usernameRequest);
    scanf("%s", clientUsername);

    //Optimize size for username
    for(int i = 0; i < MAX_ARR_LEN; i++)
    {
        if(clientUsername[i] != '\n')
        {
            continue;
        }
        else
        {
            clientUsername = (char*) realloc(clientUsername, i + 1);
        }
    }
    //Send username to server
    send(client_socket, clientUsername, sizeof(clientUsername), 0);



    //Send the client passcode to the server
    send(clientSocket, clientPasscode, sizeof(clientPasscode), 0);


    //Now ask for password
    recv(clientSocket, &pwdRequest, MAX_ARR_LEN, 0);
    printf("Server: %s\n", pwdRequest);
    scanf("%s", clientPasscode);

    //Optimize the memory allocation by removing everything after the new line terminator
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


    //Clear the screen before printing the time
    system("clear");
    printf("[%02d:%02d:%02d] %s", current_time->tm_hour, current_time->tm_min, current_time->tm_sec, pwdMessage);

    close(clientSocket);

    return 0;
}