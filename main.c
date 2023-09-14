#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define MAX_PENDING_CONNECTIONS 5
#define PORT 9002
#define SEND_FLAGS 0 
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

    int client_socket;
    int working = 1;
    char message[]="Hello world!";
    while (working) {
        // accept() blocks the caller until a connection is present
        // hence why this infinite loop works
        if ((client_socket = accept(socket_descriptor, NULL, NULL)) == -1) {
            printf("Error: some error accepting client socket\n"); // to do, look at errno
            return -1;
        }
        if (strcmp(message))
        send(client_socket, message, sizeof(message), SEND_FLAGS);
        // https://man7.org/linux/man-pages/man2/send.2.html
    }

    char bye_msg[] = "Closing server.\nBye ;)";
    send(client_socket, bye_msg, sizeof(bye_msg), SEND_FLAGS);
    close(socket_descriptor);
    return 0;
}