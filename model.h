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
    unsigned int input_n, output_n, hidden_n, hidden_n_sz;
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

    m.input_n = n_inputs;
    m.output_n = n_outputs;
    m.hidden_n = n_hidden;
    m.hidden_n_sz = sz_hidden;

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

void tearDown(struct model* Model){
    for (unsigned int i = 0; i < Model->input_n; i++) {
        printf("Input Neuron %d => \t bias: %f, output: %f\n", i, Model->input_layer.neurons[i].bias, Model->input_layer.neurons[i].output);
        free(Model->input_layer.neurons[i].weights);
    }
    free(Model->input_layer.neurons);

    for (unsigned int i = 0; i < Model->hidden_n; i++) {
        printf("Hidden Layer %d:\n", i);
        for (unsigned int j = 0; j < Model->hidden_n_sz; j++) {
            const struct neuron n = Model->hidden_layers[i].neurons[j];
            printf("\t Neuron %d => \t bias: %f, output: %f\n", j, n.bias, n.output);
            free(Model->hidden_layers[i].neurons[j].weights);
        }
        free(Model->hidden_layers[i].neurons);
    }
    free(Model->hidden_layers);

    for (unsigned int i = 0; i < Model->output_n; i++) {
        printf("Output Neuron %d => \t bias: %f, output: %f\n", i, Model->output_layer.neurons[i].bias, Model->input_layer.neurons[i].output);
        free(Model->output_layer.neurons[i].weights);
    }
    free(Model->output_layer.neurons);
}