#include "exec.h"
#include "state.h"

enum cmdres_t cmdres(struct ProgramState* state, const char* cmd)
{
    enum cmdres_t commandresult = interpret(state, cmd);
    if (commandresult == CMD_SUCCESS) return commandresult;

    switch(commandresult)
    {
        case LOAD_FAIL:
            fprintf(stderr, "Failed to load model\n");
            return commandresult;
        break;
        case LOAD_FAIL_OVERFLOW:
            fprintf(stderr, "Exceeded maximum number of models\n");
            return commandresult;
        break;
        case LOAD_FAIL_EXISTS:
            fprintf(stderr, "Requested model is already loaded\n");
            return commandresult;
        break;
        case LOAD_FAIL_FORMAT:
            fprintf(stderr, "Usage: load <model>\n");
            return commandresult;
        break;
        default:
        return commandresult;
    }

}

enum cmdres_t interpret(struct ProgramState* state, const char* cmd){
    char tokenc;
    char model[32];

    if (strncmp(cmd, "load", 4) == 0) 
    {
        tokenc = sscanf(cmd, "load %s", model);
        if (tokenc != 1) return LOAD_FAIL_FORMAT;
        return load(state, model);
    }

    if (strncmp(cmd, "store", 5) == 0) {
        printf("Storing..");
        return CMD_SUCCESS;
    }

    return CMD_SUCCESS;
}

enum cmdres_t load(struct ProgramState* state, const char* modelname)
{
    while(state->lock == 1);
    state->lock = 1;

    // for (uint8_t i = 0; i <= state->modeli; i++)
    // {
    //     if (state->modelv[i] != NULL)
    //     {
    //         if (!strcmp(state->modelv[i]->name, modelname))
    //         {
    //             state->lock = 0;
    //             return LOAD_FAIL_EXISTS;
    //         }
    //     }
    // }

    if (state->modeli >= state->modelc)
    {
        state->lock = 0;
        return LOAD_FAIL_OVERFLOW;
    }

    state->modelv[state->modeli] = loadModel(modelname);
    if (state->modelv[state->modeli] == NULL)
    {
        state->lock = 0;
        return LOAD_FAIL;
    }

    if (state->modeli < state->modelc) state->modeli++;
    state->lock = 0;

    printf("Model \"%s\" loaded.\n", modelname);
    return CMD_SUCCESS;
}