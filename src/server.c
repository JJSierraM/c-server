#include "common.h"
#include <ctype.h>
#include <time.h>

#define PING_INTERVAL 5
#define TIMEOUT_INTERVAL 12
#define MAX_CLIENTS 1024

typedef struct {
    SOCKET socket;
    time_t last_ping_time;
    time_t last_pong_time;
} Client_ping_pong;

Client_ping_pong clients[MAX_CLIENTS];
int n_clients = 0;

void send_ping();
void check_for_timeouts();

int main(int argc, char *argv[]) {

    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;

    if (argc < 2) {
        getaddrinfo(0, "34197", &hints, &bind_address);
    } else {
        if (getaddrinfo(0, argv[1], &hints, &bind_address)) {
            fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }
    }

    printf("Creating socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family,
            bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }


    printf("Binding socket to local address...\n");
    if (bind(socket_listen,
                bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);


    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    SOCKET max_socket = socket_listen;

    printf("Waiting for connections...\n");

    int open = 1;
    while(open) {
        fd_set reads;
        reads = master;
        if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }

        SOCKET i;
        for(i = 1; i <= max_socket; ++i) {
            if (FD_ISSET(i, &reads)) {

                if (i == socket_listen) {
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
                    SOCKET socket_client = accept(socket_listen,
                            (struct sockaddr*) &client_address,
                            &client_len);
                    if (!ISVALIDSOCKET(socket_client)) {
                        fprintf(stderr, "accept() failed. (%d)\n",
                                GETSOCKETERRNO());
                        return 1;
                    }

                    FD_SET(socket_client, &master);
                    if (socket_client > max_socket)
                        max_socket = socket_client;

                    char address_buffer[100];
                    getnameinfo((struct sockaddr*)&client_address,
                            client_len,
                            address_buffer, sizeof(address_buffer), 0, 0,
                            NI_NUMERICHOST);
                    printf("New connection from %s\n", address_buffer);

                    // Notify the new client about the number of other clients
                    char init_msg[1024];
                    n_clients++;
                    sprintf(init_msg, "# clients = %d\n", n_clients);
                    send(socket_client, init_msg, strlen(init_msg), 0);

                } else {
                    char read[1024];
                    int bytes_received = recv(i, read, 1024, 0);
                    if (bytes_received < 1) {
                        n_clients--;
                        FD_CLR(i, &master);
                        CLOSESOCKET(i);
                        continue;
                    }

                    if (strcmp(read_buffer, "PONG") == 0) {
                        // Find corresponding client and update last_pong_time
                        time(&clients[i].last_pong_time);
                    }

                    char read_command[6];
                    strncpy(read_command, read, 5);
                    read_command[5] = 0;
                    if (strcmp("close", read_command) == 0) {
                        open = 0;
                        break;
                    }

                    SOCKET j;
                    for (j = 1; j <= max_socket; ++j) {
                        if (FD_ISSET(j, &master)) {
                            if (j == socket_listen || j == i)
                                continue;
                            else
                                send(j, read, bytes_received, 0);
                        }
                    }
                }
            } //if FD_ISSET
        } //for i to max_socket
        //
        send_ping();
        check_for_timeouts();
    } //while(open)

    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);

    printf("Finished.\n");

    return 0;
}

void send_ping_messages(){
    time_t now;
    time(&now);
    for(int i=0; i<n_clients; ++i){
        if(difftime(now, clients[i].last_ping_time) >= PING_INTERVAL) {
            send(clients[i].socket, "PING", strlen("PING"), 0);
            clients[i].last_ping_time = now;
        }
    }
}


void check_for_timeouts() {
    time_t now;
    time(&now);

    for(int i = 0; i < client_count; ++i) {
        if (difftime(now, clients[i].last_pong_time) >= TIMEOUT_INTERVAL) {
            // Client has not responded in time, close the socket and remove from client list
            CLOSESOCKET(clients[i].socket);
            // Code to remove client from list and decrement client_count
            // Shift clients in the array, handle client_count, etc.
        }
    }
}
