#include "model.h"
#include "nnio.h"
#include "nnhost.h"

#define PORT 3200
#define BUFFER_SIZE 128

int main(int argc, char* argv[]) {

    struct server_t* server = Server(PORT, BUFFER_SIZE);
    Listen(server);
    
    return 0;

    srand(getpid());
    struct model Model = loadModel("model1");
    dumpInfo(&Model);
    tearDown(&Model);
    return 0;
}