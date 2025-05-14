#ifndef _EXEC_H_
#define _EXEC_H_

#include "io.h"
#include "state.h"


enum cmdres_t {
    CMD_SUCCESS,
    CMD_FAIL,
    CMD_FAIL_FORMAT,
    LOAD_FAIL,
    LOAD_FAIL_OVERFLOW,
    LOAD_FAIL_EXISTS,
    LOAD_FAIL_FORMAT
};

enum cmd_t {
    LOAD,
    STORE,
    LIST
};

enum cmdres_t cmdres(struct ProgramState* state, const char* cmd);
enum cmdres_t interpret(struct ProgramState* state, const char* cmd);
enum cmdres_t load(struct ProgramState* state, const char* modelname);
enum cmdres_t store(struct ProgramState* state, const char* modelname);
enum cmdres_t create(struct ProgramState* state, const char* modelname);

#endif