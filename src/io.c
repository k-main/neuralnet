#include "io.h"
#define unsigned int uint

int saveModel(struct model* Model) {
    FILE* model_file;
    model_file = fopen(Model->name, "wb");

    if (model_file == NULL) 
    {
        fprintf(stderr, "Failed to open %s\n", Model->name);
        return -1;
    }

    fwrite(&Model->output_layer.actv_func_t, sizeof(enum actvn), 1, model_file);
    fwrite(&Model->hidden_layers[0].actv_func_t, sizeof(enum actvn), 1, model_file);

    fwrite(Model, sizeof(uint), 4, model_file);
    fwrite(Model->input_layer.neurons, sizeof(struct neuron), Model->n_input, model_file);
    uint prev_layer_len;
    for (uint layer_i = 0; layer_i < Model->n_hidden; layer_i++){
        prev_layer_len = (layer_i > 0) ? Model->n_hidden_sz : Model->n_input;
        for (uint neuron_j = 0; neuron_j < Model->n_hidden_sz; neuron_j++){
            fwrite(&Model->hidden_layers[layer_i].neurons[neuron_j], sizeof(float), 2, model_file);
            fwrite(&Model->hidden_layers[layer_i].neurons[neuron_j].weights, sizeof(float), prev_layer_len, model_file);
        }
    }
    
    for (uint neuron_i = 0; neuron_i < Model->n_output; neuron_i++){
        fwrite(&Model->output_layer.neurons[neuron_i], sizeof(float), 2, model_file);
        fwrite(&Model->output_layer.neurons[neuron_i].weights, sizeof(float), Model->n_hidden_sz, model_file);
    }


    fclose(model_file);
    return 0;
}


struct model* loadModel(const char* name) {
    FILE* model_file;
    model_file = fopen(name, "rb");
    
    if (model_file == NULL)
    {
        fprintf(stderr, "Failed to open %s\n", name);
        return NULL;
    }

    struct model* Model = malloc(sizeof(struct model));
    memset(Model->name, 0, 32 * sizeof(char));
    snprintf(Model->name, 31 * sizeof(char), "%s", name);

    enum actvn activation_functions[2];
    fread(&activation_functions[0], sizeof(enum actvn), 1, model_file); // for output
    fread(&activation_functions[1], sizeof(enum actvn), 1, model_file); // for hidden
    fread(Model, sizeof(uint), 4, model_file);
    Model->input_layer.neurons = malloc(sizeof(struct neuron) * Model->n_input);
    Model->input_layer.length = Model->n_input;

    Model->output_layer.neurons = malloc(sizeof(struct neuron) * Model->n_output);
    Model->output_layer.length = Model->n_output;
    Model->output_layer.actv_func_t = activation_functions[0];
    fread(Model->input_layer.neurons, sizeof(struct neuron), Model->n_input, model_file);

    Model->hidden_layers = malloc(sizeof(struct layer) * Model->n_hidden);

    uint prev_layer_len;
    for (uint layer_i = 0; layer_i < Model->n_hidden; layer_i++){
        prev_layer_len = (layer_i > 0) ? Model->n_hidden_sz : Model->n_input;
        Model->hidden_layers[layer_i].length = Model->n_hidden_sz;
        Model->hidden_layers[layer_i].actv_func_t = activation_functions[1];
        Model->hidden_layers[layer_i].neurons = malloc(sizeof(struct neuron) * Model->n_hidden_sz);
        for (uint neuron_j = 0; neuron_j < Model->n_hidden_sz; neuron_j++){
            Model->hidden_layers[layer_i].neurons[neuron_j].weights = malloc(sizeof(float) * prev_layer_len);
            fread(&Model->hidden_layers[layer_i].neurons[neuron_j], sizeof(float), 2, model_file);
            fread(Model->hidden_layers[layer_i].neurons[neuron_j].weights, sizeof(float), prev_layer_len, model_file);
        }
    }

    for (uint neuron_i = 0; neuron_i < Model->n_output; neuron_i++){
        Model->output_layer.neurons[neuron_i].weights = malloc(sizeof(float) * Model->n_hidden_sz);
        fread(&Model->output_layer.neurons[neuron_i], sizeof(float), 2, model_file);
        fread(Model->output_layer.neurons[neuron_i].weights, sizeof(float), Model->n_hidden_sz, model_file);
    }


    return Model;
}

void dumpInfo(struct model* Model){
    printf("n_input: %d, n_output: %d, n_hidden: %d, sz_hidden: %d\n", 
    Model->n_input,
    Model->n_output,
    Model->n_hidden,
    Model->n_hidden_sz);

    struct neuron* current_neuron;
    printf("Output Activation Function: %d\n", Model->output_layer.actv_func_t);
    printf("Hidden Activation Function: %d\n", Model->hidden_layers[0].actv_func_t);
    for (uint i = 0; i < Model->n_input; i++){
        current_neuron = &Model->input_layer.neurons[i];
        printf("Loaded Input Neuron %d: bias: %f, output: %f\n", i, current_neuron->bias, current_neuron->output);
    }
    
    uint prev_layer_len;
    struct neuron* c_neuron;
    for (uint l_i = 0; l_i < Model->n_hidden; l_i++){
        prev_layer_len = (l_i > 0) ? Model->n_hidden_sz : Model->n_input;
        printf("Loaded Hidden Layer %d\n", l_i);
        for (uint n_j = 0; n_j < Model->n_hidden_sz; n_j++){
            printf("\tNeuron %d: bias: %f, output: %f, weights: ", n_j, Model->hidden_layers[l_i].neurons[n_j].bias, Model->hidden_layers[l_i].neurons[n_j].output);
            for (uint w_k = 0; w_k < prev_layer_len; w_k++) {
                printf("%f, ", Model->hidden_layers[l_i].neurons[n_j].weights[w_k]);
            }
            printf("\n");
        }
    }

    for (uint i = 0; i < Model->n_output; i++){
        current_neuron = &Model->output_layer.neurons[i];
        printf("Loaded Output Neuron %d: bias: %f, output: %f, weights: ", i, current_neuron->bias, current_neuron->output);
        for (uint w_k = 0; w_k < Model->n_hidden_sz; w_k++) {
            printf("%f, ", Model->output_layer.neurons[i].weights[w_k]);
        }
        printf("\n");
    }
}