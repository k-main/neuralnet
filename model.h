#include <stdlib.h>

struct neuron {
    float bias;
    float output;
    float* weights;
};

struct layer {
    unsigned int length;
    struct neuron* neurons;
};

struct model {
    struct layer input_layer;
    struct layer output_layer;
    struct layer* hidden_layers;
};

struct neuron Neuron(float bias, unsigned int sizeof_prev_layer) {
    struct neuron n;
    n.bias = bias;
    n.output = 0.00;
    n.weights = malloc(sizeof(float) * sizeof_prev_layer);
    return n;
}

struct layer Layer(unsigned int n_neurons, int sizeof_prev_layer){
    struct layer Layer;
    struct neuron* Neurons = malloc(n_neurons*sizeof(struct neuron));
    for (unsigned int i = 0; i < n_neurons; i++){
        Neurons[i] = Neuron(1.00, sizeof_prev_layer);
    }
    Layer.length = n_neurons;
    Layer.neurons = Neurons;
    return Layer;
}

/*Define a model -> neu*/
struct model createModel(unsigned int n_inputs, unsigned int n_outputs, unsigned int n_hidden, unsigned int sz_hidden) {
    const unsigned int n_layers = n_inputs + n_outputs + n_hidden;
    struct model m;
    m.input_layer = Layer(n_inputs, 0);
    m.output_layer = Layer(n_outputs, sz_hidden);
    struct layer* hidden_layers = malloc(n_hidden * sizeof(struct layer));

    for (unsigned int i = 0; i < n_hidden; i++) {
       switch(i){
        case 0:
            hidden_layers[i] = Layer(sz_hidden, n_inputs);
        break;
        default:
            hidden_layers[i] = Layer(sz_hidden, sz_hidden);
       }
    }

    m.hidden_layers = hidden_layers;
    return m;
}
