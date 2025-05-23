#ifndef _HOST_H_
#define _HOST_H_

#include "exec.h"

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

struct host_t {
    int sockfd;
    int port;
    int buffer_size;
    struct sockaddr_in address;
    char* input_buffer;
    char* output_buffer;
};

struct host_t* Server(int port, int buffer_size);
struct host_t* Client(int port, int buffer_size);
void closeHost(struct host_t* server);
int Listen(struct host_t* server, struct ProgramState* state);
int Connect(struct host_t*, const char* host);

#endif