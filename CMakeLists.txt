cmake_minimum_required(VERSION 4.0)
project(neuralnet
        VERSION 0.0.0.0
        DESCRIPTION "Neural network from scratch in C"
        )
set(CMAKE_C_FLAGS "-lm")
add_executable(nn nn.c nnhost.h nnhost.c model.h model.c nnio.h nnio.c)
add_executable(nncli nncli.c)