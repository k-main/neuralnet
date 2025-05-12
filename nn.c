#include "model.h"
#include "nnio.h"
#include "nnhost.h"

int main(int argc, char* argv[]) {
    uint8_t remote_host = 0;
    int PORT = 3200;
    int BUFFER_SIZE = 128;

    for (uint32_t argi = 1; argi < argc; argi++) {
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
                    fprintf(stderr, "[nnhost] Server port = %d\n", PORT);
                    argi++;
                }
            }
        }
    }
        
    if (remote_host == 1) {
        struct host_t* server = Server(PORT, BUFFER_SIZE);
        if (server != NULL) {
            Listen(server);
        } 
    } 

    char input_buffer[BUFFER_SIZE];
    uint32_t getline_n = BUFFER_SIZE * sizeof(char) - 1;
    char* chars_read;

    do {
        fprintf(stderr, "> ");
        memset(input_buffer, 0, strlen(input_buffer) * sizeof(char) + 1);
        chars_read = fgets(input_buffer, BUFFER_SIZE * sizeof(char), stdin);
        if (chars_read == NULL) {
            fprintf(stderr, "Error reading from stdin.\n");
            return -1;
        }
        fprintf(stderr, "< %s", input_buffer);

        // pass to interpreter

    } while(strncmp(input_buffer, "exit", 4) != 0);
    
    return 0;

    srand(getpid());
    struct model Model = loadModel("model1");
    dumpInfo(&Model);
    tearDown(&Model);
    return 0;
}
