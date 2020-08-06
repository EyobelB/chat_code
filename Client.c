#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_ARR_LEN 256

//callback for pthread_create, receives messages from the server
void* receiveMessages(void* server_socket)
{
    char message[MAX_ARR_LEN];
    int socket = *(int*)server_socket;

    //loops until a message is received and then prints it out
    while(recv(socket, message, MAX_ARR_LEN, 0)>0)
    {
        printf("%s\n", message);
    }
    return NULL;
}

//ASSUME CLIENT CODE HERE
int main()
{
    //create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //specify an address for the client
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;
    int connection_status;
    connection_status = connect(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    //checks if the client connected successfully
    if(connection_status == -1)
    {
        printf("Failed to connect");
    }

    //Initialize the pthread
    pthread_t thread;

    //create a pointer to the server_socket for the receiveMessages function
    int *tmp = malloc(sizeof(*tmp));
    *tmp = server_socket;

    //creates the thread that runs receiveMessages while the current thread runs the while loop
    pthread_create(&thread, NULL, receiveMessages, tmp);
    while(1) 
    {
        char messageToSend[MAX_ARR_LEN];

        //receive a message from the user, removes the newline, and checks for the exit character, "~"
        fgets(messageToSend, MAX_ARR_LEN, stdin);
        strtok(messageToSend, "\n");
        if(strcmp("~", messageToSend) == 0)
        {
            break;
        }

        //sends the trimmed message to the server
        send(server_socket, messageToSend, MAX_ARR_LEN, 0);
    }


    close(server_socket);
    return 0;
}