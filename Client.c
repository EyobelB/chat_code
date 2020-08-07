#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
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
    //Define time related data
    time_t timeTrackerInSeconds, val = 1;
    struct tm* current_time;
    timeTrackerInSeconds = time(NULL);
    current_time = localtime(&timeTrackerInSeconds);

    //create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //specify an address for the client
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;
    int connection_status;
    connection_status = connect(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    //checks if the client connected successfully
    if(connection_status == -1)
    {
        printf("Server: There was an error making a connection to the remote socket.");
    }

    //

    //Initialize the pthread
    pthread_t thread;

    //create a pointer to the server_socket for the receiveMessages function
    int *tmp = malloc(sizeof(*tmp));
    *tmp = server_socket;

    //creates the thread that runs receiveMessages while the current thread runs the while loop
    pthread_create(&thread, NULL, receiveMessages, tmp);
    short verified = 0;
    char serverStatus[23];
    char usernamePrompt[23];
    char* username = calloc(MAX_ARR_LEN, sizeof(char));
    char* password = calloc(MAX_ARR_LEN, sizeof(char));
    char passwordPrompt[25];
    char acceptancePrompt[25];


    while(1)
    {
        char messageToSend[MAX_ARR_LEN];

        //Check verification of passcode
        if(verified == 0)
        {
            recv(server_socket, &serverStatus, sizeof(serverStatus), 0);
            recv(server_socket, &usernamePrompt, sizeof(usernamePrompt), 0);

            printf("Server: %s\n", usernamePrompt);
            //Scan for a username
            fgets(username, MAX_ARR_LEN, stdin);
            //Send username to the server after reallocation
            for(int i  = 0; i < MAX_ARR_LEN; i++)
            {
              if(username[i] != '\n')
              {
                continue;
              }
              else
              {
                username = (char*) realloc(username, i + 1);
              }
            }
            //Now send username
            send(server_socket, username, sizeof(username), 0);

            //Now start password handling
            recv(server_socket, &passwordPrompt, MAX_ARR_LEN, 0);
            printf("Server: %s\n", passwordPrompt);
            scanf("%s", password);

            //Optimize the memory allocation of password
            for(int i = 0; i < MAX_ARR_LEN; i++)
            {
              if(password[i] != '\n')
              {
                continue;
              }
              else
              {
                password = (char*) realloc(password, i + 1);
              }
            }

            //Send passcode to server
            send(server_socket, password, sizeof(password), 0);

            //Receieve the acceptance prompt
            recv(server_socket, &acceptancePrompt, sizeof(acceptancePrompt), 0);

            //Prepare screen for future posts
            system("clear");
            //Print the time
            printf("[%02d:%02:%02d] Server: %s", current_time->tm_hour, current_time->tm_min, current_time->tm_sec, acceptancePrompt);
            verified = 1;
        }

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