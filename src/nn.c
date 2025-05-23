#include "io.h"
#include "model.h"
#include "host.h"
#include "exec.h"

#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {
    srand(getpid());
    struct ProgramState* state = defineState(4);
    char result[BUFFER_SIZE];

    unsigned char remote_host = 0;
    int PORT = 3200;
    

    for (int argi = 1; argi < argc; argi++) {
        if (!strncmp(argv[argi], "--host", 6)) {
            remote_host = 1;
            continue;
        }

        if (remote_host) {
            if (!strncmp(argv[argi], "--port", 6)) 
            {
                if (argi == argc - 1) {
                    fprintf(stderr, "[nnhost] Expected port\n");
                    return 1;
                } else {
                    PORT = atoi(argv[argi + 1]);
                    if (!PORT) {
                        fprintf(stderr, "[nnhost] Invalid port: %d\n", argv[argi + 1]);
                        return 1;
                    }
                    printf("[nnhost] Server port = %d\n", PORT);
                    argi++;
                }
            }
        }
    }
        
    if (remote_host == 1) {
        struct host_t* server = Server(PORT, BUFFER_SIZE);
        if (server != NULL) {
            Listen(server, state);

            memset(result, 0, sizeof(char) * BUFFER_SIZE);
            cmdres(state, "destroy", result, BUFFER_SIZE);
            printf(result);

            return 0;
        } 
    } 

    char input_buffer[BUFFER_SIZE];
    int getline_n = BUFFER_SIZE * sizeof(char) - 1;
    char* chars_read;

    do {
        printf("> ");
        memset(result, 0, BUFFER_SIZE * sizeof(char));
        memset(input_buffer, 0, strlen(input_buffer) * sizeof(char) + 1);
        chars_read = fgets(input_buffer, BUFFER_SIZE * sizeof(char), stdin);
        if (chars_read == NULL) {
            fprintf(stderr, "Error reading from stdin.\n");
            return -1;
        }

        cmdres(state, input_buffer, result, BUFFER_SIZE);
        printf(result);
    } while(strncmp(input_buffer, "exit", 4) != 0);

    memset(result, 0, sizeof(char) * BUFFER_SIZE);
    cmdres(state, "destroy", result, BUFFER_SIZE);
    printf(result);
    return 0;
}