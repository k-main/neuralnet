cmake_minimum_required(VERSION 4.0)
project(neuralnet
        VERSION 0.0.0.0
        DESCRIPTION "Neural network from scratch in C"
        )
set(CMAKE_C_FLAGS "-lm -g")
add_executable(nn src/nn.c src/model.h src/model.c src/io.h src/io.c src/exec.h src/exec.c src/host.h src/host.c)
add_executable(nncli src/nncli.c src/host.h src/host.c src/exec.h src/exec.c src/model.h src/model.c src/io.h src/io.c)