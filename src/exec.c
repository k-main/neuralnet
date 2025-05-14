#include "exec.h"

enum cmdres_t cmdres(struct ProgramState* state, const char* cmd)
{
    enum cmdres_t commandresult = interpret(state, cmd);
    if (commandresult == CMD_SUCCESS) return commandresult;

    switch(commandresult)
    {
        case CMD_FAIL_FORMAT:
            fprintf(stderr, "Unrecognized command\n");
            return commandresult;
        break;
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
        case STORE_FAIL:
            fprintf(stderr, "Failed to store model\n");
            return commandresult;
        break;
        case CMD_FAIL_NOEXIST:
            fprintf(stderr, "The specified model was not found\n");
            return commandresult;
        break;
        case STORE_FAIL_FORMAT:
            fprintf(stderr, "Usage store <model>\n");
            return commandresult;
        break;
        case CREATE_FAIL:
            fprintf(stderr, "Failedto create model\n");
            return commandresult;
        break;
        case CREATE_FAIL_FORMAT:
            fprintf(stderr, "Usage create --name <name> --inputs <i> --outputs <o> --hidden <h> --hlen <h len>\n");
            return commandresult;
        break;
        case CREATE_FAIL_EXISTS:
            fprintf(stderr, "A model with the same name is already loaded\n");
            return commandresult;
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

    if (strncmp(cmd, "store", 5) == 0) 
    {
        tokenc = sscanf(cmd, "store %s", model);
        if (tokenc != 1) return STORE_FAIL_FORMAT;
        return store(state, model);
    }

    if (strncmp(cmd, "list", 4) == 0)
    {
        return list(state);
    }

    struct ModelParams params;

    if (strncmp(cmd, "create", 6) == 0)
    {
        tokenc = sscanf(cmd, 
        "create --name %s --inputs %d --outputs %d --hidden %d --hlen %d", 
        params.name, &params.n_inputs, &params.n_outputs, &params.n_hidden, &params.n_hidden_sz);
        if (tokenc != 5) {
            return CREATE_FAIL_FORMAT;
        }
        return create(state, &params);
    }

    return CMD_FAIL_FORMAT;
    
    if (strncmp(cmd, "unload", 6) == 0)
    {
        tokenc = sscanf(cmd, "unload %s", model);
        if (tokenc != 1) {
            return UNLOAD_FAIL_FORMAT;
        }

        return unload(state, model);
    }

}

enum cmdres_t load(struct ProgramState* state, const char* modelname)
{
    while(state->lock == 1);
    state->lock = 1;

    for (uint8_t i = 0; i <= state->modeli; i++)
    {
        if (state->modelv[i] != NULL)
        {
            if (!strcmp(state->modelv[i]->name, modelname))
            {
                state->lock = 0;
                return LOAD_FAIL_EXISTS;
            }
        }
    }

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

enum cmdres_t store(struct ProgramState* state, const char* modelname)
{
    while(state->lock == 1);
    state->lock = 1;

    int retc;

    for (uint8_t i = 0; i < state->modeli; i++)
    {
        if(!strcmp(state->modelv[i]->name, modelname))
        {
            retc = saveModel(state->modelv[i]);
            state->lock = 0;
            if (retc == 0)
            {
                printf("Model \"%s\" saved.", modelname);
                return CMD_SUCCESS;
            }
            return STORE_FAIL;
        }
    }
    
    state->lock = 0;
    return CMD_FAIL_NOEXIST;
}

enum cmdres_t list(struct ProgramState* state)
{
    while(state->lock == 1);
    state->lock = 1;

    if (state->modeli == 0)
    {
        state->lock = 0;
        printf("No models loaded\n");
        return CMD_SUCCESS;
    }

    for (uint8_t i = 0; i < state->modeli; i++)
    {
        printf("[%d] %s\n", i + 1, state->modelv[i]->name);
    }
    
    state->lock = 0;
    return CMD_SUCCESS;
}

enum cmdres_t create(struct ProgramState* state, struct ModelParams* params)
{
    while(state->lock == 1)
    state->lock = 1;

    enum actvn f_activation = relu;

    if (state->modeli >= 4)
    {
        state->lock = 0;
        return LOAD_FAIL_OVERFLOW;
    }

    for (uint8_t i = 0; i < state->modeli; i++)
    {
        if (strcmp(params->name, state->modelv[i]->name) == 0)
        {
            state->lock = 0;
            return CREATE_FAIL_EXISTS;
        }
    }

    state->modelv[state->modeli] = createModel(
        params->name,
        params->n_inputs,
        params->n_outputs,
        params->n_hidden,
        params->n_hidden_sz,
        relu
    );

    if (state->modelv[state->modeli] != NULL)
    {
        printf(
            "Created model \"%s\" with input size %d, output size %d, and %d hidden layers of size %d\n",
            params->name, params->n_inputs, params->n_outputs, params->n_hidden, params->n_hidden_sz
        );
        state->modeli++;
    } else {
        state->lock = 0;
        return CREATE_FAIL;
    }

    state->lock = 0;
    return CMD_SUCCESS;
}

enum cmdres_t unload(struct ProgramState* state, const char* modelname)
{
    while(state->lock == 1);
    state->lock = 1;
    
    for (uint8_t i = 0; i < state->modeli; i++)
    {
        if (strcmp(state->modelv[i]->name, modelname) == 0)
        {
            tearDown(state->modelv[i]);
            printf("Unloaded model \"%s\".\n", modelname);
            state->modeli--;
            state->lock = 0;
            return CMD_SUCCESS;
        }
    }

    state->lock = 0;
    return CMD_FAIL_NOEXIST;
}