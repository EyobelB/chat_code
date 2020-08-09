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

int client_sockets[MAX_CLIENTS];
char username[MAX_CLIENTS][MAX_ARR_LEN];
char password[MAX_ARR_LEN] = "yemane12";
time_t rawtime;
struct tm * timeinfo;

/*
callback function for pthread_create that recieves
messages from the clients and sends them to all the other clients
*/
void* forwardMessages(void* client_socket)
{


    int socket = client_sockets[*(int*)client_socket];
    int userNum;
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        if(client_sockets[i] == socket)
        {
            userNum = i;
        }

    }



    char message[MAX_ARR_LEN];

    int iterator = 0;

    //loops until a message is received
    while (recv(socket, &message, MAX_ARR_LEN, 0)>0)
    {
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        char timeStamp[MAX_ARR_LEN];
        sprintf(timeStamp, "[%02d:%02d:%02d] ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

        if(iterator == 0)
         {
             strcpy(message, "Joined Server!");
         }
         iterator++;


        //concatenates the sender's socket to them message and prints to the server console
        char sender[MAX_ARR_LEN];
        sprintf(sender, "%s: ", username[*(int*)client_socket]);
        char serverTimeStamp[MAX_ARR_LEN];
        strcpy(serverTimeStamp, timeStamp);
        printf("%s\n", strcat(serverTimeStamp, strcat(sender, message)));

        //loops through all the clients
        for(int i = 0; i < MAX_CLIENTS; i++)
        {

            //checks to make sure the message is sent to all but the original sender
            if(client_sockets[i] != socket)
            {
                //concatenates the sender's socket and sends to the client
                char colon[MAX_ARR_LEN];
                sprintf(colon, "%s: ", username[*(int*)client_socket]);
                send(client_sockets[i], strcat(timeStamp, strcat(colon, message)), MAX_ARR_LEN, 0);
            }

        }
    }
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char timeStamp[MAX_ARR_LEN];
    sprintf(timeStamp, "[%02d:%02d:%02d] ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    printf("%s%s has left\n", timeStamp, username[userNum]);
    return NULL;
}

int main()
{

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

    //loops until a new client connection is found and accepted
    while((client_sockets[n] = accept(serverSocket, (struct sockaddr *)NULL, NULL)))
    {
        //initializes a new pthread and creates a pointer for the client's socket
        pthread_t thread;
        int *tmp = malloc(sizeof(*tmp));
        *tmp = n;



        recv(client_sockets[n], &username[n], MAX_ARR_LEN, 0);

        char userPassword[MAX_ARR_LEN];
        recv(client_sockets[n], &userPassword, MAX_ARR_LEN, 0);

        if(strcmp(password, userPassword) != 0)
        {
            send(client_sockets[n], "Sorry, wrong password...", 25, 0);
            close(client_sockets[n]);
        }
        else
        {
            char welcomeMessage[MAX_ARR_LEN];
            sprintf(welcomeMessage, "Welcome to the server, %s!\n", username[n]);
            send(client_sockets[n], welcomeMessage, MAX_ARR_LEN, 0);
        }

        //creates the pthread and runs the forwardMessages function
        pthread_create(&thread, NULL, forwardMessages, tmp);

        n++;
    }


    //close the socket now
    close(serverSocket);

    return 0;
}
