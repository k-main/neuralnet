#include <stdio.h>
#include "neuron.h"

int main(int argc, char* argv[]) {

    struct layer l = Layer(3);

    l.neurons[0].bias = 1.00;
    l.neurons[0].output = 2.00;
    l.neurons[1].bias = 3.00;
    l.neurons[1].output = 4.00;
    l.neurons[2].bias = 5.00;
    l.neurons[2].output = 6.00;

    // printf("sizeof(neuron) = %d\n", sizeof(struct neuron));

    for (unsigned int i = 0; i < l.length; i ++) {
        printf("Neuron %d bias: %f output: %f\n", i, l.neurons[i].bias, l.neurons[i].output);
    }
    

    return 0;
}