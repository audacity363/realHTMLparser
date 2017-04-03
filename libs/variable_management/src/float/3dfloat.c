#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "var_management.h"

int new3DFloat(VariableObject *anker, char *group, char *name, int x_length, int y_length, int z_length)
{
    VariableObject *target = NULL;

    if((target = addNewVariable(anker, group, name, THREEDFLOAT)) == NULL)
        return(-1);

    target->size = ((x_length*sizeof(double))*y_length)*z_length;

    target->array_length[0] = x_length;
    target->array_length[1] = y_length;
    target->array_length[2] = z_length;

    if((target->data = malloc(target->size)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    memset(target->data, target->size, 0x00);
    return(0);

}

//TODO: Add Range check!!!! On ever set... function!!!
int set3DFloatXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z, double val)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if(target->type != THREEDFLOAT)
    {
        rh4n_errno = WRONG_VAR_TYPE;
        return(-1);
    }

    int offset = OFFSET_3DFloat(target->array_length[0], 
        target->array_length[1], x, y, z);
    ((double*)target->data)[offset] = val;
    return(0);
}

double get3DFloatXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(0);

    return(((double*)target->data)[OFFSET_3DFloat(target->array_length[0],
        target->array_length[1], x, y, z)]);
}

int print3DFloat(VariableObject *target, FILE *output, int mode)
{
    int offset = 0, i = 0, x = 0, z = 0;

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
            fprintf(output, "[");
            for(z=0; z < target->array_length[2]; z++)
            {
                offset = OFFSET_3DFloat(target->array_length[0], 
                    target->array_length[1], i, x, z);
                fprintf(output, "%f", ((double*)target->data)[offset]);
                if(z+1 < target->array_length[2])
                    fprintf(output, ", ");
            }
            fprintf(output, "]");
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
