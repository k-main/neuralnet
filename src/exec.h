#ifndef _EXEC_H_
#define _EXEC_H_

#include "io.h"
#include "model.h"

struct ProgramState {
    uint8_t lock;
    uint8_t modelc;
    uint8_t modeli;
    struct model* modelv[4];
};

struct ModelParams {
    uint32_t n_inputs;
    uint32_t n_outputs;
    uint32_t n_hidden;
    uint32_t n_hidden_sz;
    char name[64];
};

enum cmdres_t {
    CMD_SUCCESS,
    CMD_FAIL,
    CMD_FAIL_FORMAT,
    CMD_FAIL_NOEXIST,
    LOAD_FAIL,
    LOAD_FAIL_OVERFLOW,
    LOAD_FAIL_EXISTS,
    LOAD_FAIL_FORMAT,
    UNLOAD_FAIL_FORMAT,
    STORE_FAIL,
    STORE_FAIL_FORMAT,
    CREATE_FAIL,
    CREATE_FAIL_FORMAT,
    CREATE_FAIL_EXISTS,
};

enum cmdres_t cmdres(struct ProgramState* state, const char* cmd);
enum cmdres_t interpret(struct ProgramState* state, const char* cmd);
enum cmdres_t info(struct ProgramState* state, const char* modelname);
enum cmdres_t load(struct ProgramState* state, const char* modelname);
enum cmdres_t unload(struct ProgramState* state, const char* modelname);
enum cmdres_t store(struct ProgramState* state, const char* modelname);
enum cmdres_t create(struct ProgramState* state, struct ModelParams* params);
enum cmdres_t list(struct ProgramState* state);

#endif