#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "var_management.h"

int new2DFloat(VariableObject *anker, char *group, char *name, int x_length, int y_length)
{
    VariableObject *target = NULL;

    if((target = addNewVariable(anker, group, name, TWODFLOAT)) == NULL)
        return(-1);

    target->size = (x_length*sizeof(double))*y_length;

    target->array_length[0] = x_length;
    target->array_length[1] = y_length;

    if((target->data = malloc(target->size)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    memset(target->data, target->size, 0x00);
    return(0);

}

int set2DFloatXY(VariableObject *anker, char *group, char *name, int x, int y, double val)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if(target->type != TWODFLOAT)
    {
        rh4n_errno = WRONG_VAR_TYPE;
        return(-1);
    }
    
    *((double*)(target->data+OFFSET_2DFloat(target->array_length[1], x, y))) = val;
    return(0);
}

double get2DFloatXY(VariableObject *anker, char *group, char *name, int x, int y)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(0);

    return(*((double*)((target->data+OFFSET_2DFloat(target->array_length[1], x, y)))));
}

int print2DFloat(VariableObject *target, FILE *output, int mode)
{
    int offset = 0, i = 0, x = 0;

    if(mode == PRINT_MODE_FORMAT)
        fprintf(output, "[%s] = ", target->name);
    else if(mode == PRINT_MODE_JSON)
        fprintf(output, "\"%s\": ", target->name);

    fprintf(output, "[");
    for(; i < target->array_length[0]; i++)
    {
        fprintf(output, "[");
        for(x=0; x < target->array_length[1]; x++)
        {
            offset = OFFSET_2DFloat(target->array_length[1], i, x);
            fprintf(output, "%f", *((double*)(target->data+offset)));
            if(x+1 < target->array_length[1])
                fprintf(output, ", ");
        }
        fprintf(output, "]");
        if(i+1 < target->array_length[0])
            fprintf(output, ", ");
    }
    fprintf(output, "]");
    return(0);
}
