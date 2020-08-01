#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;



    //bind the socket to the specified IP and port
    bind(serverSocket, (struct sockaddr*) &server_address, sizeof(server_address));

    //Second parameter on listen determines how many clients to listen for
    listen(serverSocket, 1);

    //Start accepting client connections
    int client_socket;
    char passcode[MAX_ARR_LEN] = "yemane12";
    char clientPasscode[MAX_ARR_LEN];
    int isKicked;


    //Accept for now
    client_socket = accept(serverSocket, NULL, NULL);

    send(client_socket, server_message, sizeof(server_message), 0);
    send(client_socket, "Enter the passcode below", MAX_ARR_LEN, 0);

    //Receive the client passcode, one byte at a time
    recv(client_socket, &clientPasscode, MAX_ARR_LEN, 0);


    //Check for password compliance
    if(strcmp(passcode, clientPasscode) != 0)
    {
        send(client_socket, "Sorry, wrong password...", MAX_ARR_LEN, 0);
    }
    else
    {
        send(client_socket, "Welcome to the server!", MAX_ARR_LEN, 0);
    }


    close(serverSocket);



    return 0;
}