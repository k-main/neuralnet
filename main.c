#include <stdio.h>
#include "model.h"

int main(int argc, char* argv[]) {

    const unsigned int input_n = 3, output_n = 3, hidden_n = 3, sz_hidden = 5;
    struct model Model = createModel(input_n, output_n, hidden_n, sz_hidden);

    tearDown(&Model);

    return 0;
}