#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PENDING_CONNECTIONS 5
#define PORT 9002
#define SEND_FLAGS 0

// void close_on_ctrl_c(int _);
int main()
{
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0); // see refernce @ https://pubs.opengroup.org/onlinepubs/7908799/xns/socket.html

    struct sockaddr_in server_parameters = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    bind(socket_descriptor, (struct sockaddr*)&server_parameters, sizeof(server_parameters));
    // https://stackoverflow.com/questions/18609397/whats-the-difference-between-sockaddr-sockaddr-in-and-sockaddr-in6:w

    if (listen(socket_descriptor, MAX_PENDING_CONNECTIONS) == -1) {
        // https://man7.org/linux/man-pages/man2/listen.2.html
        printf("Error: could not listen\n");
        return -1;
    }
    //signal(SIGINT, close_on_ctrl_c);

    int client_socket;
    char message[] = "Hello from the server!";
    char client_msg[256];
    for (;;) {
        // accept() blocks the caller until a connection is present
        // hence why this infinite loop works
        if ((client_socket = accept(socket_descriptor, NULL, NULL)) == -1) {
            printf("Error: some error accepting client socket\n"); // to do, look at errno
            return -1;
        }
        send(client_socket, message, sizeof(message), SEND_FLAGS);
        // https://man7.org/linux/man-pages/man2/send.2.html

        bzero(client_msg, 256);
        if(read(client_socket, client_msg, sizeof(char) * 10) == -1){
          perror("Error reading from client");
        }
        printf("%s", client_msg);
        if (strncmp(client_msg, "bye", 3) == 0) { // exit on possible client response.
            break;
        }
        
    }

    char bye_msg[] = "Closing server.\nBye ;)\n";
    send(client_socket, bye_msg, sizeof(bye_msg), SEND_FLAGS);
    close(socket_descriptor);
    printf("Exiting server\n");
    return 0;
}

/*
void close_on_ctrl_c(int _)
{
    goto exit;
}
*/