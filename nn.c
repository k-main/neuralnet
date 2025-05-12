#include "model.h"
#include "nnio.h"
#include "nnhost.h"

#define PORT 3201
#define BUFFER_SIZE 128

int main(int argc, char* argv[]) {

    struct host_t* server = Server(PORT, BUFFER_SIZE);
    if (server != NULL) {
        Listen(server);
    } 
    
    return 0;

    srand(getpid());
    struct model Model = loadModel("model1");
    dumpInfo(&Model);
    tearDown(&Model);
    return 0;
}
