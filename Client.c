#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARR_LEN 256



//ASSUME CLIENT CODE HERE
int main()
{
    //Create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //Specify an address for the client
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6667);
    server_address.sin_addr.s_addr = INADDR_ANY;
    int connection_status;
    while((connection_status = connect(server_socket, (struct sockaddr *) &server_address, sizeof(server_address))))
    {
        printf("Tring to connect...\n");
    }

    int exit = 0;
    char username[MAX_ARR_LEN] = {};
    while(exit == 0) 
    {
        char messageToSend[MAX_ARR_LEN];
        printf("%s: ", username);
        fgets(messageToSend, MAX_ARR_LEN, stdin);
        strtok(messageToSend, "\n");
        if(strcmp("~", messageToSend) == 0)
        {
            return 0;
        }
        send(server_socket, messageToSend, MAX_ARR_LEN, 0);

    }
    

    
    

    close(server_socket);
    return 0;
}