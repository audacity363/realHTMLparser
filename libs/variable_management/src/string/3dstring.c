#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "var_management.h"

int new3DString(VariableObject *anker, char *group, char *name, int length, 
    int x_length, int y_length, int z_length)
{
    VariableObject *target = NULL;

    if((target = addNewVariable(anker, group, name, THREEDSTRING)) == NULL)
        return(-1);

    target->size = ((((length+1)*sizeof(wchar_t))*z_length)*y_length)*x_length;


    target->array_length[0] = x_length;
    target->array_length[1] = y_length;
    target->array_length[2] = z_length;
    target->length = length;

    if((target->data = malloc(target->size)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    memset(target->data, 0x00, target->size);
    return(0);
}

int newStringfrom3DString(VariableObject *anker, char *group, char *name, int x,
    int y, int z, char *new_group, char *new_name)
{
    VariableObject *target = NULL, *new_target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if((new_target = addNewVariable(anker, new_group, new_name, STRING)) == NULL)
        return(-1);

    SET_FLAG(target->flags, RH4N_FLG_COPY);

    new_target->length = target->length;
    new_target->size = (target->length+1)*sizeof(wchar_t);

    new_target->data = target->data+OFFSET_3DString(target->length, 
        target->array_length[1], target->array_length[2], x, y, z);

    return(0);
}

int set3DStringXYZ(VariableObject *anker, char *group, char *name, int x, int y, 
    int z, wchar_t *val)
{
    VariableObject *target = NULL;
    int offset = 0;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if(x >= target->array_length[0])
    {
        rh4n_errno = X_INDEX_OUT_OF_RANGE;
        return(-1);
    }
    if(y >= target->array_length[1])
    {
        rh4n_errno = X_INDEX_OUT_OF_RANGE;
        return(-1);
    }
    if(wcslen(val) > target->length)
    {
        rh4n_errno = STRING_TO_LONG;
        return(-1);
    }

    offset = OFFSET_3DString(target->length, target->array_length[1], 
        target->array_length[2], x, y, z);

    wcscpy((wchar_t*)(target->data+offset), val);

    return(0);
}

wchar_t *get3DStringXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z)
{
    VariableObject *target = NULL;
    int offset = 0;

    if((target = getVariable(anker, group, name)) == NULL)
        return(NULL);

    if(x >= target->array_length[0])
    {
        rh4n_errno = X_INDEX_OUT_OF_RANGE;
        return(NULL);
    }

    offset = OFFSET_3DString(target->length, target->array_length[1], 
        target->array_length[2], x, y, z);

    return((wchar_t*)(target->data+offset));
}

int print3DString(VariableObject *target, FILE *output, int mode)
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
                offset = OFFSET_3DString(target->length, target->array_length[1], 
                    target->array_length[2], i, x, z);
                fprintf(output, "\"%S\"", (wchar_t*)(target->data+offset));
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

int print3DStringXYZ(VariableObject *target, int x, int y, int z, FILE *output)
{
    int offset = 0;

    if(x >= target->array_length[0])
    {
        rh4n_errno = X_INDEX_OUT_OF_RANGE;
        return(-1);
    }
    if(y >= target->array_length[1])
    {
        rh4n_errno = X_INDEX_OUT_OF_RANGE;
        return(-1);
    }

    offset = OFFSET_3DString(target->length, target->array_length[1], 
        target->array_length[2], x, y, z);
    fprintf(output, "%S", (wchar_t*)(target->data+offset));
    return(0);
}

