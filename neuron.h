

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
    struct layer hidden_layers[];
};

struct layer Layer(unsigned int n_neurons){
    struct layer Layer;
    struct neuron Neurons[n_neurons];
    Layer.length = n_neurons;
    Layer.neurons = Neurons;
    return Layer;
}

struct model neuralNet(unsigned int n_inputs, unsigned int n_outputs, unsigned int n_hidden) {

}
