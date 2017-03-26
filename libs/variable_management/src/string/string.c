#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "var_management.h"

int newString(VariableObject *anker, char *group, char *name, int length)
{
    VariableObject *target = NULL;

    if((target = addNewVariable(anker, group, name, STRING)) == NULL)
        return(-1);

    target->size = (length+1)*sizeof(wchar_t);
    target->length = length;

    if((target->data = malloc(target->size)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    memset(target->data, target->size, 0x00);
    return(0);
}

int setString(VariableObject *anker, char *group, char *name, wchar_t *val)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(-1);

    if(wcslen(val) > target->length)
    {
        rh4n_errno = STRING_TO_LONG;
        return(-1);
    }

    wcscpy((wchar_t*)(target->data), val);
    return(0);
}

wchar_t *getString(VariableObject *anker, char *group, char *name)
{
    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
        return(NULL);

    return((wchar_t*)(target->data));
}

int printString(VariableObject *target, FILE *output, int mode)
{
    if(mode == PRINT_MODE_FORMAT)
        fprintf(output, "[%s] = [%S]", target->name, (wchar_t*)target->data);
    else if(mode == PRINT_MODE_JSON)
        fprintf(output, "\"%s\": \"%S\"", target->name, (wchar_t*)target->data);
    else if(mode == PRINT_MODE_RAW)
        fprintf(output, "%S", (wchar_t*)target->data);

}
