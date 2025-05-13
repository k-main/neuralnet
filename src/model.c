#include "model.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define def_edge_weight 1.00
#define def_neur_output 1.00
#define def_neur_biasvl 1.00
#define uint unsigned int

float _rand(void) {
    return ((rand() > RAND_MAX / 2) ? 1.00 : -1.00)*((float)rand() / (float)RAND_MAX);
}

float activation(enum actvn func, float input) {
    switch(func){
        case relu:
            return (input > 0.00) ? input : 0.00;
        break;
        case sigmoid:
            return 1.0 / (1.0 + exp((double)(-input)));
        break;
        default:
            return input;
    }
}

struct neuron Neuron(float bias, unsigned int sizeof_prev_layer) {
    struct neuron n;
    n.bias = bias;
    n.output = def_neur_output;
    n.weights = malloc(sizeof(float) * sizeof_prev_layer);
    float rand_weight;
    for (unsigned int i = 0; i < sizeof_prev_layer; i++) {
        rand_weight = _rand();
        n.weights[i] = rand_weight;
    }
    return n;
}

struct layer Layer(unsigned int n_neurons, int sizeof_prev_layer, enum actvn func){
    struct layer Layer;
    struct neuron* Neurons = malloc(n_neurons*sizeof(struct neuron));
    for (unsigned int i = 0; i < n_neurons; i++){
        Neurons[i] = Neuron(_rand(), sizeof_prev_layer);
    }
    Layer.length = n_neurons;
    Layer.neurons = Neurons;
    Layer.actv_func_t = func;
    return Layer;
}

struct model createModel(char* name, unsigned int n_inputs, unsigned int n_outputs, unsigned int n_hidden, unsigned int sz_hidden, enum actvn actv_func) {
    const unsigned int n_layers = n_inputs + n_outputs + n_hidden;
    struct model m;

    m.name = name;
    m.n_input = n_inputs;
    m.n_output = n_outputs;
    m.n_hidden = n_hidden;
    m.n_hidden_sz = sz_hidden;

    m.input_layer = Layer(n_inputs, 0, actv_func);
    m.output_layer = Layer(n_outputs, sz_hidden, actv_func);
    struct layer* hidden_layers = malloc(n_hidden * sizeof(struct layer));

    for (unsigned int i = 0; i < n_hidden; i++) {
       switch(i){
        case 0:
            hidden_layers[i] = Layer(sz_hidden, n_inputs, actv_func);
        break;
        default:
            hidden_layers[i] = Layer(sz_hidden, sz_hidden, actv_func);
       }
    }

    m.hidden_layers = hidden_layers;
    return m;
}

void setHiddenActFunc(struct model* Model, enum actvn func) {
    for (unsigned int layer_i = 0; layer_i < Model->n_hidden; layer_i++) {
        Model->hidden_layers[layer_i].actv_func_t = func;
    }
}
void setOutputActFunc(struct model* Model, enum actvn func) {
    Model->output_layer.actv_func_t = func;
}

void computeLayer(struct layer* prv_layer, struct layer* cur_layer) {
    struct neuron* current_neuron;
    float neuron_output;

    for ( unsigned int neur_i = 0; neur_i < cur_layer->length; neur_i++ ){
        neuron_output = 0.00;
        current_neuron = &cur_layer->neurons[neur_i];
        for ( unsigned int prv_neur_j = 0; prv_neur_j < prv_layer->length; prv_neur_j++ ) {
            neuron_output += (prv_layer->neurons[prv_neur_j].output * current_neuron->weights[prv_neur_j]);
        }
        current_neuron->output = activation(cur_layer->actv_func_t, (neuron_output + current_neuron->bias));
    }
}

float* genResult(struct model* Model){
    float* out_values = malloc(Model->n_output * sizeof(float));
    struct layer* current_layer;
    struct layer* prev_layer;

    for ( unsigned int layer_i = 0; layer_i < Model->n_hidden; layer_i++ ){
        current_layer = &Model->hidden_layers[layer_i];
        prev_layer = (layer_i == 0) ? &Model->input_layer : &Model->hidden_layers[layer_i - 1];
        computeLayer(prev_layer, current_layer);
    }
    
    computeLayer(&Model->hidden_layers[Model->n_hidden - 1], &Model->output_layer);

    for ( unsigned int k = 0; k < Model->n_output; k++ ){
        out_values[k] = Model->output_layer.neurons[k].output;
    }

    return out_values;
}

void tearDown(struct model* Model){
    free(Model->input_layer.neurons);
    unsigned int prev_layer_length = Model->n_input;

    for (unsigned int i = 0; i < Model->n_hidden; i++) {
        for (unsigned int j = 0; j < Model->n_hidden_sz; j++) {
            free(Model->hidden_layers[i].neurons[j].weights);
        }
        prev_layer_length = Model->n_hidden_sz;
        free(Model->hidden_layers[i].neurons);
    }
    free(Model->hidden_layers);

    for (unsigned int i = 0; i < Model->n_output; i++) {
        free(Model->output_layer.neurons[i].weights);
    }
    free(Model->output_layer.neurons);
}

