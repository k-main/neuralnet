#include "io.h"

int saveModel(struct model* Model) {
    FILE* model_fptr;
    model_fptr = fopen(Model->name, "wb");

    fwrite(Model, sizeof(int), 4, model_fptr); // write n_input,output,hidden,hidden_sz

    for (unsigned int neuron_i = 0; neuron_i < Model->n_input; neuron_i++) {
        fwrite(&Model->input_layer.neurons[neuron_i].bias, sizeof(float), 1, model_fptr);
        fwrite(&Model->input_layer.neurons[neuron_i].output, sizeof(float), 1, model_fptr);
    }
    
    struct neuron* current_neuron;
    for (unsigned int layer_i = 0; layer_i < Model->n_hidden; layer_i++){
        current_neuron = &Model->hidden_layers[layer_i].neurons[neuron_i];
        if (layer_i > 0) {

        } else {
            for (unsigned int neuron_i = 0; neuron_i < Model->n_input; neuron_i++){
                fwrite(current_neuron->bias, sizeof(float), 1, model_fptr);
                fwrite(current_neuron->output, sizeof(float), 1, model_fptr);
                fwrite(current_neuron->weights, sizeof(float)*n_input, model_fptr);
            }
        }
    }

    fclose(model_fptr);
}


void loadModel(const char* name) {
    FILE* model_fptr;
    model_fptr = fopen(name, "rb");
    unsigned int* modelparam_buffer = malloc(4*sizeof(int));
    struct model Model;
    
    fread(modelparam_buffer, sizeof(int), 4, model_fptr);
    Model.name = name;
    Model.n_input = modelparam_buffer[0];
    Model.n_output = modelparam_buffer[1];
    Model.n_hidden = modelparam_buffer[2];
    Model.n_hidden_sz = modelparam_buffer[3];
    free(modelparam_buffer);
    
    float* buffer = malloc(2 * sizeof(float));
    Model.input_layer.neurons = malloc(sizeof(Model.n_input) * sizeof(struct neuron));
    Model.hidden_layers = malloc(sizeof(struct layer) * Model.n_hidden);

    for (unsigned int neuron_i = 0; neuron_i < Model.n_input; neuron_i++){
        fread(buffer, sizeof(float), 2, model_fptr);
        Model.input_layer.neurons[neuron_i].bias = buffer[0];
        Model.input_layer.neurons[neuron_i].output = buffer[1];
    }
    free(buffer);

    printf("n_input: %d, n_output: %d, n_hidden: %d, sz_hidden: %d\n", 
        Model.n_input,
        Model.n_output,
        Model.n_hidden,
        Model.n_hidden_sz);

    for (unsigned int neuron_i = 0; neuron_i < Model.n_input; neuron_i++){
        printf("Input Neuron %d => bias: %f, output: %f\n", neuron_i, Model.input_layer.neurons[neuron_i].bias, Model.input_layer.neurons[neuron_i].output);
    }


}