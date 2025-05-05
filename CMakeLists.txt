cmake_minimum_required(VERSION 4.0)
project(neuralnet
        VERSION 0.0.0.0
        DESCRIPTION "Neural network from scratch in C"
        )
set(CMAKE_C_FLAGS "-lm")
add_executable(nn neuralnet.c model.h model.c io.h io.c)
add_executable(nncli, neuralnetcli.c)