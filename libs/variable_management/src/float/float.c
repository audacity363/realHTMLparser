#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "var_management.h"

int newFloat(VariableObject *anker, char *group, char *name)
{
    VariableObject *target = NULL;

    if((target = addNewVariable(anker, group, name, FLOAT)) == NULL)
        return(-1);

    target->size = sizeof(double);

    if((target->data = malloc(target->size)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    memset(target->data, target->size, 0x00);
    return(0);

}

int setFloat(VariableObject *anker, char *group, char *name, double val)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if(target->type != FLOAT)
    {
        rh4n_errno = WRONG_VAR_TYPE;
        return(-1);
    }
    
    *((double*)target->data) = val;
    return(0);

}

double getFloat(VariableObject *anker, char *group, char *name)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(0);

    return(*((double*)(target->data)));
}

int printFloat(VariableObject *target, FILE *output, int mode)
{
    if(mode == PRINT_MODE_FORMAT)
        fprintf(output, "[%s] = [%f]", target->name, *((double*)target->data));
    else if(mode == PRINT_MODE_JSON)
        fprintf(output, "\"%s\": \"%f\"", target->name, *((double*)target->data));
    else if(mode == PRINT_MODE_RAW)
        fprintf(output, "%f", *((double*)target->data));
}
