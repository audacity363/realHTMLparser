#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "utils.h"

int addString(vars_t *anker, char *group, char *name, wchar_t *val, int length)
{
    vars_t *end, *new, *grp;
    int ret;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = STRING;
    if((new->data = malloc((length+1)*sizeof(wchar_t))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    wcscpy((wchar_t*)new->data, val);
    return(0);
}
