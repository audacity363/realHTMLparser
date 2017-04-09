#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "var_management.h"

int newBoolean(VariableObject *anker, char *group, char *name)
{
    VariableObject *target = NULL;

    if((target = addNewVariable(anker, group, name, BOOL)) == NULL)
        return(-1);

    target->size = sizeof(bool);

    if((target->data = malloc(target->size)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    memset(target->data, 0x00, target->size);
    return(0);
}

int setBoolean(VariableObject *anker, char *group, char *name, bool val)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if(target->type != BOOL)
    {
        rh4n_errno = WRONG_VAR_TYPE;
        return(-1);
    }
    
    *((bool*)target->data) = val;
    return(0);
}

bool getBoolean(VariableObject *anker, char *group, char *name)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(0);

    return(*((bool*)(target->data)));
}

int printBoolean(VariableObject *target, FILE *output, int mode)
{
    if(mode == PRINT_MODE_FORMAT)
        fprintf(output, "[%s] = [%s]", target->name, 
            (*((bool*)target->data) == true) ? "true" : "false" );
    else if(mode == PRINT_MODE_JSON)
        fprintf(output, "\"%s\": \"%s\"", target->name,
            (*((bool*)target->data) == true) ? "true" : "false" );
    else if(mode == PRINT_MODE_RAW)
        fprintf(output, "%s", 
            (*((bool*)target->data) == true) ? "true" : "false" );
}
