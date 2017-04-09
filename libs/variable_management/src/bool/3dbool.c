#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "var_management.h"

int new3DBoolean(VariableObject *anker, char *group, char *name, int x_length, int y_length, int z_length)
{
    VariableObject *target = NULL;

    if((target = addNewVariable(anker, group, name, THREEDBOOL)) == NULL)
        return(-1);

    target->size = ((sizeof(bool)*x_length)*y_length)*z_length;
    target->array_length[0] = x_length;
    target->array_length[1] = y_length;
    target->array_length[2] = z_length;

    if((target->data = malloc(target->size)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    memset(target->data, 0x00, target->size);
    return(0);
}

int set3DBooleanXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z, bool val)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if(target->type != THREEDBOOL)
    {
        rh4n_errno = WRONG_VAR_TYPE;
        return(-1);
    }
    
    *((bool*)(target->data+OFFSET_3DBoolean(target->array_length[0],
        target->array_length[1], target->array_length[2], x, y, z))) = val;
    return(0);
}

bool get3DBooleanXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(0);

    return(*((bool*)(target->data+OFFSET_3DBoolean(target->array_length[0],
        target->array_length[1], target->array_length[2], x, y, z))));
}

int print3DBoolean(VariableObject *target, FILE *output, int mode)
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
                offset = OFFSET_3DBoolean(target->array_length[0], 
                    target->array_length[1], target->array_length[2], i, x, z);
                fprintf(output, "%s", 
                    (*((bool*)(target->data+offset)) == true) ? "true" : "false" );
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
