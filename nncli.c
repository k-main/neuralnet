#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memcpy

#include "nnhost.h"

#define PORT 3201
#define BUFFER_SIZE 128

int main(int argc, char* argv[]){

    struct host_t* client = Client(PORT, BUFFER_SIZE);
    const char* host = "127.0.0.1";
    Connect(client, host);
    return 0;
}