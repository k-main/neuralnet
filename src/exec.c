#include "exec.h"

struct ProgramState* defineState(uint8_t modelc)
{
    struct ProgramState* state = malloc(sizeof(struct ProgramState));
    state->lock = 0;
    state->modelc = modelc;
    state->modeli = 0;
    state->modelv = malloc(modelc * sizeof(struct model*));
    return state;
}

enum cmdres_t cmdres(struct ProgramState* state, const char* cmd, char* buffer, uint32_t buffer_size)
{
    enum cmdres_t commandresult = interpret(state, cmd, buffer, buffer_size);
    if (commandresult == CMD_SUCCESS) return commandresult;

    switch(commandresult)
    {
        case CMD_FAIL_FORMAT:
            snprintf(buffer, buffer_size, "Unrecognized command\n");
            return commandresult;
        break;
        case LOAD_FAIL:
            snprintf(buffer, buffer_size, "Failed to load model\n");
            return commandresult;
        break;
        case LOAD_FAIL_OVERFLOW:
            snprintf(buffer, buffer_size, "Exceeded maximum number of models\n");
            return commandresult;
        break;
        case LOAD_FAIL_EXISTS:
            snprintf(buffer, buffer_size, "Requested model is already loaded\n");
            return commandresult;
        break;
        case LOAD_FAIL_FORMAT:
            snprintf(buffer, buffer_size, "Usage: load <model>\n");
            return commandresult;
        break;
        case STORE_FAIL:
            snprintf(buffer, buffer_size, "Failed to store model\n");
            return commandresult;
        break;
        case CMD_FAIL_NOEXIST:
            snprintf(buffer, buffer_size, "The specified model was not found\n");
            return commandresult;
        break;
        case STORE_FAIL_FORMAT:
            snprintf(buffer, buffer_size, "Usage store <model>\n");
            return commandresult;
        break;
        case CREATE_FAIL:
            snprintf(buffer, buffer_size, "Failedto create model\n");
            return commandresult;
        break;
        case CREATE_FAIL_FORMAT:
            snprintf(buffer, buffer_size, "Usage create --name <name> --inputs <i> --outputs <o> --hidden <h> --hlen <h len>\n");
            return commandresult;
        break;
        case CREATE_FAIL_EXISTS:
            snprintf(buffer, buffer_size, "A model with the same name is already loaded\n");
            return commandresult;
        default:
        return commandresult;
    }

}

enum cmdres_t interpret(struct ProgramState* state, const char* cmd, char* buffer, uint32_t buffer_size){
    char tokenc;
    char model[32];
    enum cmdres_t result;


    if (strncmp(cmd, "load", 4) == 0) 
    {
        tokenc = sscanf(cmd, "load %s", model);
        if (tokenc != 1) return LOAD_FAIL_FORMAT;
        result = load(state, model);
        if (result == CMD_SUCCESS)
        {
            snprintf(buffer, buffer_size, "Model \"%s\" loaded.\n", model);
        }
        return result;
    }

    if (strncmp(cmd, "store", 5) == 0) 
    {
        tokenc = sscanf(cmd, "store %s", model);
        if (tokenc != 1) return STORE_FAIL_FORMAT;
        result = store(state, model);
        if (result == CMD_SUCCESS)
        {
            snprintf(buffer, buffer_size * sizeof(char), "Model \"%s\" saved.\n", model);
        }
        return result;
    }

    if (strncmp(cmd, "list", 4) == 0)
    {
        result = list(state, buffer, buffer_size);
        return result;
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
        result = create(state, &params);
        if (result == CMD_SUCCESS)
        {
            snprintf(buffer, buffer_size * sizeof(char), 
            "Created model \"%s\"\n %d input neurons\n %d output neurons\n %d hidden layers with %d neurons each\n"
            , params.name, params.n_inputs, params.n_outputs, params.n_hidden, params.n_hidden_sz);
        }
        return result;
    }

    if (strncmp(cmd, "destroy", 7) == 0)
    {
        result = destroy(state, buffer, buffer_size);
        if (result == CMD_SUCCESS)
        {
            snprintf(buffer + strlen(buffer), (buffer_size - strlen(buffer)), "\nSuccessfully deallocated all structures.\n");
        }
        return result;
    }

    if (strncmp(cmd, "exit", 4) == 0)
    {
        snprintf(buffer, buffer_size, "Exiting..\n");
        return CMD_SUCCESS;
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
                return CMD_SUCCESS;
            }
            return STORE_FAIL;
        }
    }
    
    state->lock = 0;
    return CMD_FAIL_NOEXIST;
}

enum cmdres_t list(struct ProgramState* state, char* buffer, uint32_t buffer_size)
{
    while(state->lock == 1);
    state->lock = 1;

    if (state->modeli == 0)
    {
        state->lock = 0;
        snprintf(buffer, buffer_size, "No models loaded\n");
        return CMD_SUCCESS;
    }

    char current_message[64] = "\0";
    uint8_t len;
    for (uint8_t i = 0; i < state->modeli; i++)
    {  
        len = strlen(current_message);
        snprintf(current_message + (len * sizeof(char)), (64 - len) * sizeof(char), "[%d] %s\n", i + 1, state->modelv[i]->name);
    }
    
    snprintf(buffer, buffer_size * sizeof(char), "%s", current_message);

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

enum cmdres_t destroy(struct ProgramState* state, char* buffer, uint32_t buffer_size)
{
    while(state->lock == 1);
    state->lock = 0;

    for (uint8_t i = 0; i < state->modeli; i++)
    {
        if (state->modelv[i] != NULL)
        {
            snprintf(buffer + strlen(buffer), (buffer_size - strlen(buffer)) * sizeof(char), "Clearing model %d, \"%s\" from program state\n", i, state->modelv[i]->name);
            tearDown(state->modelv[i]);
        }
    }

    free(state->modelv);
    free(state);

    return CMD_SUCCESS;
}