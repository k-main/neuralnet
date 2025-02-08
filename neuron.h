#ifndef NEURON_H
#define NEURON_H
#include <vector>
#include "linear.h"

class Neuron {
    private:
        std::vector<double> inputs;
        std::vector<double> weights;
        double bias;
        double output;
    public:
        Neuron() : bias(0), output(0) {};
        Neuron(std::vector<double> Inputs, std::vector<double> Weights, double Bias) : inputs(Inputs), weights(Weights), bias(Bias) {};
        double computeOutput();
};

class Layer {
    public:


};

#endif 