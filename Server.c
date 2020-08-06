#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define MAX_ARR_LEN 256


int main()
{
    char server_message[MAX_ARR_LEN] = "Successfully connected!";

    int serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);


    //define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6667);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //bind the socket to the specified IP and port
    bind(serverSocket, (struct sockaddr*) &server_address, sizeof(server_address));

    //Second parameter on listen determines how many clients to listen for
    listen(serverSocket, 5);

    //Start accepting client connections
    int client_socket;
    while((client_socket = accept(serverSocket, (struct sockaddr *)NULL, NULL))) 
    {
        pid_t pid;
        char message[MAX_ARR_LEN] = "";
        if((pid = fork()) == 0) 
        {
            while (recv(client_socket, &message, MAX_ARR_LEN, 0)>0) 
            {
                printf("%d: %s\n", client_socket, message);
            }
        }
}


    //close the socket now
    close(serverSocket);

    return 0;
}

