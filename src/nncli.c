#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memcpy

#include "host.h"

#define BUFFER_SIZE 128
#define HOSTLEN 64

char** cmdt;
uint32_t cmdtc = 0;

int main(int argc, char* argv[]){
    char host[HOSTLEN] = "127.0.0.1";
    int32_t PORT = 3200;

    if (argc > 1) {
        for (uint32_t argi = 1; argi < argc; argi++) 
        {
            if (!strcmp(argv[argi], "--host")) 
            {
                if (argi == argc - 1) 
                {
                    fprintf(stderr, "Expected host address.\n");
                    return -1;
                } else {
                    snprintf(host, sizeof(char) * HOSTLEN, "%s", argv[argi + 1]);
                    argi++;
                }
                continue;
            }

            if (!strcmp(argv[argi], "--port")) 
            {
                if (argi == argc - 1) 
                {
                    fprintf(stderr, "Expected port.\n");
                    return -1;
                } else {
                    PORT = atoi(argv[argi + 1]);
                    if (PORT == 0) {
                        fprintf(stderr, "Invalid port.\n");
                        return -1;
                    }
                    argi++;
                }
                continue;
            }

            if (!strcmp(argv[argi], "-i")) 
            {
                if (argi == argc - 1) {
                    fprintf(stderr, "Expected command.\n");
                    return -1;
                } else {
                    cmdt = argv + sizeof(char) * (argi + 1);
                    cmdtc = argc - argi - 1;
                    break;
                }
            }
        }
    }

    struct host_t* client = Client(PORT, BUFFER_SIZE);
    Connect(client, host);
    return 0;
}