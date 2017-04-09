#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "var_management.h"

int newInteger(VariableObject *anker, char *group, char *name)
{
    VariableObject *target = NULL;

    if((target = addNewVariable(anker, group, name, INTEGER)) == NULL)
        return(-1);

    target->size = sizeof(int);

    if((target->data = malloc(target->size)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    memset(target->data, 0x00, target->size);
    return(0);

}

int setInteger(VariableObject *anker, char *group, char *name, int val)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if(target->type != INTEGER)
    {
        rh4n_errno = WRONG_VAR_TYPE;
        return(-1);
    }
    
    *((int*)target->data) = val;
    return(0);

}

int getInteger(VariableObject *anker, char *group, char *name)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(0);

    return(*((int*)(target->data)));
}

int printInteger(VariableObject *target, FILE *output, int mode)
{
    if(mode == PRINT_MODE_FORMAT)
        fprintf(output, "[%s] = [%d]", target->name, *((int*)target->data));
    else if(mode == PRINT_MODE_JSON)
        fprintf(output, "\"%s\": %d", target->name, *((int*)target->data));
    else if(mode == PRINT_MODE_RAW)
        fprintf(output, "%d", *((int*)target->data));
}
