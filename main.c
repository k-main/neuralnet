#include "model.h"

int main(int argc, char* argv[]) {

    const unsigned int input_n = 1, output_n = 1, hidden_n = 1, sz_hidden = 3;
    struct model Model = createModel(input_n, output_n, hidden_n, sz_hidden);

    float* result = genResult(&Model);
    
    tearDown(&Model);
    free(result);
    return 0;
}