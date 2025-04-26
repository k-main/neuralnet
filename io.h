#include "model.h"
#ifndef io_h_
#define io_h_

int saveModel(struct model* Model);
void loadModel(const char* name);
void dumpInfo(struct model* Model);
#endif