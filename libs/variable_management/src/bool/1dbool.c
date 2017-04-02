#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "var_management.h"

int new1DBoolean(VariableObject *anker, char *group, char *name, int x_length)
{
    VariableObject *target = NULL;

    if((target = addNewVariable(anker, group, name, ONEDBOOL)) == NULL)
        return(-1);

    target->size = sizeof(bool)*x_length;
    target->array_length[0] = x_length;

    if((target->data = malloc(target->size)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    memset(target->data, target->size, 0x00);
    return(0);
}

int set1DBooleanX(VariableObject *anker, char *group, char *name, int x, bool val)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if(target->type != ONEDBOOL)
    {
        rh4n_errno = WRONG_VAR_TYPE;
        return(-1);
    }
    
    *((bool*)target->data+OFFSET_1DBoolean(x)) = val;
    return(0);
}

bool get1DBooleanX(VariableObject *anker, char *group, char *name, int x)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(0);

    return(*((bool*)(target->data+OFFSET_1DBoolean(x))));
}

int print1DBoolean(VariableObject *target, FILE *output, int mode)
{
    int offset = 0, i = 0;

    if(mode == PRINT_MODE_FORMAT)
        fprintf(output, "[%s] = ", target->name);
    else if(mode == PRINT_MODE_JSON)
        fprintf(output, "\"%s\": ", target->name);

    fprintf(output, "[");
    for(; i < target->array_length[0]; i++)
    {
        offset = OFFSET_1DBoolean(i);
        fprintf(output, "%s", 
            (*((bool*)target->data+offset) == true) ? "true" : "false" );
        if(i+1 < target->array_length[0])
            fprintf(output, ", ");
    }
    fprintf(output, "]");
    return(0);
}
