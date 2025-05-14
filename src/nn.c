#include "io.h"
#include "host.h"
#include "exec.h"
#include "state.h"

int main(int argc, char* argv[]) {
    srand(getpid());
    struct ProgramState state;

    state.lock = 0;
    state.modeli = 0;
    state.modelc = 4;

    char remote_host = 0;
    int PORT = 3200;
    int BUFFER_SIZE = 128;

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
            Listen(server);
            return 0;
        } 
    } 

    char input_buffer[BUFFER_SIZE];
    int getline_n = BUFFER_SIZE * sizeof(char) - 1;
    char* chars_read;

    do {
        printf("> ");
        memset(input_buffer, 0, strlen(input_buffer) * sizeof(char) + 1);
        chars_read = fgets(input_buffer, BUFFER_SIZE * sizeof(char), stdin);
        if (chars_read == NULL) {
            fprintf(stderr, "Error reading from stdin.\n");
            return -1;
        }

        cmdres(&state, input_buffer);

    } while(strncmp(input_buffer, "exit", 4) != 0);

    for (uint8_t i = 0; i < state.modeli; i++)
    {
        if (state.modelv[i] != NULL)
        {
            printf("Deleting state.modelv[%d] \"%s\"\n", i, state.modelv[i]->name);
            tearDown(state.modelv[i]);
            free(state.modelv[i]);
        }
    }

    return 0;
}
