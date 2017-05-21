#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "var_management.h"

int new1DFloat(VariableObject *anker, char *group, char *name, int x_length)
{
    VariableObject *target = NULL;

    if((target = addNewVariable(anker, group, name, ONEDFLOAT)) == NULL)
        return(-1);

    target->size = x_length*sizeof(double);

    target->array_length[0] = x_length;

    if((target->data = malloc(target->size)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    memset(target->data, 0x00, target->size);
    return(0);

}

int set1DFloatX(VariableObject *anker, char *group, char *name, int x, double val)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if(target->type != ONEDFLOAT)
    {
        rh4n_errno = WRONG_VAR_TYPE;
        return(-1);
    }
    
    *((double*)(target->data+OFFSET_1DFloat(x))) = val;
    return(0);
}

double get1DFloatX(VariableObject *anker, char *group, char *name, int x)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(0);

    return(*((double*)((target->data+OFFSET_1DFloat(x)))));
}

int print1DFloat(VariableObject *target, FILE *output, int mode)
{
    int offset = 0, i = 0;

    if(mode == PRINT_MODE_FORMAT)
        fprintf(output, "[%s] = ", target->name);
    else if(mode == PRINT_MODE_JSON)
        fprintf(output, "\"%s\": ", target->name);

    fprintf(output, "[");
    for(; i < target->array_length[0]; i++)
    {
        offset = OFFSET_1DFloat(i);
        fprintf(output, "%f", *((double*)(target->data+offset)));
        if(i+1 < target->array_length[0])
            fprintf(output, ", ");
    }
    fprintf(output, "]");
    return(0);
}
