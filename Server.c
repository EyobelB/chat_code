#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_ARR_LEN 256
#define MAX_CLIENTS 5

//Initialize the maximum amount of clients, and password length
int client_sockets[MAX_CLIENTS];
const char password[MAX_ARR_LEN] = "yemane12";
char usernames[MAX_ARR_LEN][MAX_CLIENTS];

//Define time related data
time_t timeTrackerInSeconds, val = 1;
struct tm* current_time;


/*
callback function for pthread_create that recieves
messages from the clients and sends them to all the other clients
*/
void* forwardMessages(void* client_socket)
{
    int socket = *(int*)client_socket;
    char message[MAX_ARR_LEN];

    //loops until a message is received
    while (recv(socket, &message, MAX_ARR_LEN, 0)>0)
    {
        char usernameToUse[MAX_ARR_LEN];
        //concatenates the sender's socket to them message and prints to the server console
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
          if(client_sockets[i] == socket)
          {
            *usernameToUse = usernames[MAX_ARR_LEN][i];
            break;
          }
        }
        char fullMessage[MAX_ARR_LEN];
        sprintf(fullMessage, "[%02d:%02d:%02d] %s: ", current_time->tm_hour, current_time->tm_min, current_time->tm_sec, usernameToUse);
        printf("%s\n", strcat(fullMessage, message));

        //loops through all the clients
        for(int i = 0; i < MAX_CLIENTS; i++)
        {

            //checks to make sure the message is sent to all but the original sender
            if(client_sockets[i] != socket)
            {
                //concatenates the sender's socket and sends to the client
                char messageToClients[MAX_ARR_LEN];
                sprintf(messageToClients, "[%02d:%02d:%02d] %s: ", current_time->tm_hour, current_time->tm_min, current_time->tm_sec, usernameToUse);
                send(client_sockets[i], strcat(messageToClients, message), MAX_ARR_LEN, 0);
            }

        }
    }
    printf("%d has left\n", socket);
    return NULL;
}

int main()
{

    timeTrackerInSeconds = time(NULL);
    current_time = localtime(&timeTrackerInSeconds);

    int serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);


    //define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //bind the socket to the specified IP and port
    bind(serverSocket, (struct sockaddr*) &server_address, sizeof(server_address));

    //second parameter on listen determines how many clients to listen for
    listen(serverSocket, MAX_CLIENTS);

    //iterator for the client_sockets array
    int n = 0;

    //Temporarily accept each client, prompting for a username and password
    //Create a set of values to verify
    short verifyLogin[MAX_CLIENTS];

    for(int i = 0; i < MAX_CLIENTS; i++)
    {
      verifyLogin[i] = 0;
    }

    //loops until a new client connection is found and accepted
    while((client_sockets[n] = accept(serverSocket, (struct sockaddr *)NULL, NULL)))
    {
        //initializes a new pthread and creates a pointer for the client's socket
        if(verifyLogin[n] == 0)
        {
          char enteredPassword[MAX_ARR_LEN];
          send(client_sockets[n], "Successfully Connected", 23, 0);

          send(client_sockets[n], "Enter a username below", 23, 0);
          //Receive the username
          recv(client_sockets[n], &usernames[0][n], MAX_ARR_LEN, 0);

          send(client_sockets[n], "Enter the password below", 25, 0);
          //Receive a password
          recv(client_sockets[n], &enteredPassword, MAX_ARR_LEN, 0);

          //Check password for compliance
          if(strcmp(password, enteredPassword))
          {
            send(client_sockets[n], "Sorry, wrong password...", 25, 0);
            close(client_sockets[n]);
          }
          else
          {
            char* welcome = "Welcome to the server ";
            strcat(welcome, usernames[0][n]);
            send(client_sockets[n], welcome, 23, 0);
          }
          verifyLogin[n] = 1;
        }
        pthread_t thread;
        int *tmp = malloc(sizeof(*tmp));
        *tmp = client_sockets[n];

        //creates the pthread and runs the forwardMessages function
        pthread_create(&thread, NULL, forwardMessages, tmp);

        n++;
    }


    //close the socket now
    close(serverSocket);

    return 0;
}