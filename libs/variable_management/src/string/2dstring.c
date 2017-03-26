#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "var_management.h"

int new2DString(VariableObject *anker, char *group, char *name, int length, int x_length, int y_length)
{
    VariableObject *target = NULL;

    if((target = addNewVariable(anker, group, name, TWODSTRING)) == NULL)
        return(-1);

    target->size = (((length+1)*sizeof(sizeof(wchar_t))) * x_length)*y_length;
    target->array_length[0] = x_length;
    target->array_length[1] = y_length;
    target->length = length;

    if((target->data = malloc(target->size)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    memset(target->data, target->size, 0x00);
    return(0);
}

int newStringfrom2DString(VariableObject *anker, char *group, char *name, int x,
    char *new_group, char *new_name)
{
    VariableObject *target = NULL, *new_target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if((new_target = addNewVariable(anker, new_group, new_name, STRING)) == NULL)
        return(-1);

    SET_FLAG(target->flags, RH4N_FLG_COPY);

    new_target->length = target->length;
    new_target->size = (target->length+1)*sizeof(wchar_t);

    new_target->data = target->data;

    return(0);
}

int set2DStringXY(VariableObject *anker, char *group, char *name, int x, int y, wchar_t *val)
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

    offset = OFFSET_2DString(target->length, target->array_length[1], x, y);
    wcscpy((wchar_t*)(target->data+offset), val);

    return(0);
}

wchar_t *get2DStringX(VariableObject *anker, char *group, char *name, int x, int y)
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

    offset = OFFSET_2DString(target->length, target->array_length[1], x, y);

    return((wchar_t*)(target->data+offset));
}

int print2DString(VariableObject *target, FILE *output, int mode)
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
            offset = OFFSET_2DString(target->length, target->array_length[1], i, x);
            fprintf(output, "\"%S\"", (wchar_t*)(target->data+offset));
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

int print2DStringXY(VariableObject *target, int x, int y, FILE *output)
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

    offset = OFFSET_2DString(target->length, target->array_length[1], x, y);
    fprintf(output, "%S", (wchar_t*)(target->data+offset));
    return(0);
}

