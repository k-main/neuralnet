#ifndef _STATE_H_
#define _STATE_H_

#include "model.h"

struct ProgramState {
    uint8_t lock;
    uint8_t modelc;
    uint8_t modeli;
    struct model* modelv[4];
};

#endif