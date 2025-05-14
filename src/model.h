#ifndef _MODEL_H_
#define _MODEL_H_

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>

struct neuron {
    float bias;
    float output;
    float* weights;
};

enum actvn {
    linear,
    relu,
    sigmoid
};

struct layer {
    unsigned int length;
    enum actvn actv_func_t;
    struct neuron* neurons;
};

struct model {
    unsigned int n_input, n_output, n_hidden, n_hidden_sz;
    struct layer input_layer;
    struct layer output_layer;
    char name[32];
    struct layer* hidden_layers;
};

float _rand(void);
struct neuron Neuron(float bias, unsigned int sizeof_prev_layer);
struct layer Layer(unsigned int n_neurons, int sizeof_prev_layer, enum actvn func);
struct model createModel(char* name, unsigned int n_inputs, unsigned int n_outputs, unsigned int n_hidden, unsigned int n_hidden_sz, enum actvn actv_func);
void setHiddenActFunc(struct model* Model, enum actvn func);
void setOutputActFunc(struct model* Model, enum actvn func);
void computeLayer(struct layer* prev_layer, struct layer* cur_layer);
float* genResult(struct model* Model);
void tearDown(struct model* Model);
#endif