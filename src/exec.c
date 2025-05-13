#include "exec.h"
#include "state.h"

enum cmd_ret_t interpret(const char* cmd){
    unsigned char tokenc;
    char Model[32];

    if (strncmp(cmd, "load", 4) == 0) 
    {
        tokenc = sscanf(cmd, "load %s", Model);
        if (tokenc != 1) {
            return CMD_FAIL;
        }
        
        loadedModels[modelc] = loadModel(Model);
        if (loadedModels[modelc] == NULL){
            fprintf(stderr, "Failed to load model \"%s\". \n", Model);
            return CMD_FAIL;
        } else {
            printf("Loaded model \"%s\" into memory.\n", loadedModels[modelc]->name);
            modelc++;
        }
        
        
        return CMD_SUCCESS;
    }

    if (strncmp(cmd, "store", 5) == 0) {
        printf("Storing..");
        return CMD_SUCCESS;
    }

    return CMD_SUCCESS;
}
