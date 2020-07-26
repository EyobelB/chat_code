#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>



//ASSUME CLIENT CODE HERE
int main()
{
    //Create a socket
    int socketData;
    socketData = socket(AF_INET, SOCK_STREAM, 0);

    //Specify an address for the client
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(socketData, (struct sockaddr *) &server_address, sizeof(server_address));

    //Check for potential connection errors
    if(connection_status == -1)
    {
        printf("Server: There was an error in making a connection to the remote socket.");
    }

    char message[256];
    scanf("%s", message);
    

    send(socketData, message, sizeof(message), 0);

    
    //This code will receive(recv) data from a server
    //char server_output[256]; //String that holds response from server
    //recv(socketData, &server_output, sizeof(server_output), 0);

    //print out data that has been received from the server
    //printf("Server: %s", server_output);
    

    //finally, close the socket.
    close(socketData);
    return 0;
}