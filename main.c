#include "model.h"
#include "io.h"

int main(int argc, char* argv[]) {
    srand(getpid());
    // const unsigned int input_n = 3, output_n = 3, hidden_n = 1, sz_hidden = 3;
    // struct model Model = createModel("model1", input_n, output_n, hidden_n, sz_hidden, relu);
    // setOutputActFunc(&Model, sigmoid);
    // float* result = genResult(&Model);
    // saveModel(&Model);
    struct model Model = loadModel("model1");
    // dumpInfo(&Model);
    // free(result);
    tearDown(&Model);
    return 0;
}