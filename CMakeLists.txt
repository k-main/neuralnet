cmake_minimum_required(VERSION 3.1)
project(neuralnet
        VERSION 0.0.0.0
        DESCRIPTION "Neural network from scratch in C++"
        )
add_executable(main main.cpp neuron.h linear.h)