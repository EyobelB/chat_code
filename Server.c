#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>


int main()
{
    char server_message[256] = "Successfully connected!";

    int serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);


    //define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //bind the socket to the specified IP and port
    bind(serverSocket, (struct sockaddr*) &server_address, sizeof(server_address));

    //Second parameter on listen determines how many clients to listen for
    listen(serverSocket, 1);

    //Start accepting client connections
    int client_socket;
    client_socket = accept(serverSocket, NULL, NULL);
   

    //Send a server message
    //send(client_socket, server_message, sizeof(server_message), 0);
    
    char message_output[256];
    recv(client_socket, &message_output, sizeof(message_output), 0);
    printf("%s", message_output);

    //close the socket now
    close(serverSocket);

    return 0;
}

