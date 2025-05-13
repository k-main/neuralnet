#ifndef io_h_
#define io_h_
#include "model.h"

int saveModel(struct model* Model);
struct model* loadModel(const char* name);
void dumpInfo(struct model* Model);
#endif