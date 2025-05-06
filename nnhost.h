#ifndef nnhost_h_
#define nnhost_h_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

/*
    Minimalistic interface for creating a simple TCP server to communicate
    with the program daemon from a separate docker container.

    I just wanted main() to look cleaner.
*/

struct server_t {
    int sockfd;
    int port;
    int buffer_size;
    struct sockaddr_in address;
    char* input_buffer;
    char* output_buffer;
};

#define unsigned int uint

struct server_t* Server(int port, int buffer_size);
void closeServer(struct server_t* server);
int Listen(struct server_t* server);
#endif