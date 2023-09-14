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

    connect(socket_descriptor, (struct sockaddr*)&server_parameters, sizeof(server_parameters));
    char client_msg[] = "Hello world\0";
    char server_msg[256];

    read(socket_descriptor, server_msg, sizeof(server_msg));
    // write(socket_descriptor, client_msg, sizeof(client_msg));

    printf("Sever says: %s\n", server_msg);

    char bye[] = "bye\0";
    write(socket_descriptor, bye, sizeof(bye));
    // See: <https://stackoverflow.com/questions/1790750/what-is-the-difference-between-read-and-recv-and-between-send-and-write>
    close(socket_descriptor);
    return 0;
}

/*
void close_on_ctrl_c(int _)
{
    goto exit;
}
*/