#ifndef _EXEC_H_
#define _EXEC_H_

#include "io.h"

enum cmd_ret_t {
    CMD_SUCCESS,
    CMD_FAIL
};

enum cmd_t {
    LOAD,
    STORE,
    LIST
};

enum cmd_ret_t interpret(const char* cmd);

#endif