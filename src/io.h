#ifndef _IO_H_
#define _IO_H_
#include "model.h"

int saveModel(struct model* Model);
struct model* loadModel(const char* name);
void dumpInfo(struct model* Model);

#endif