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

int add1DStringArray(vars_t *anker, char *group, char *name, int var_length, int x_length)
{
    vars_t *end, *new, *grp;
    int ret, size;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = ONEDSTRING;
    new->length = var_length+1;
    new->x_length = x_length;
    size = ((var_length+1)*sizeof(wchar_t))*x_length;
    if((new->data = malloc(size)) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    return(0);
}


int edit1DStringArray(vars_t *anker, char *group, char *name, wchar_t *val, int x_index)
{
    vars_t *target, *grp;
    int index;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    if(wcslen(val)+1 >= target->length)
        return(VAR_VALUE_TO_LONG);        

    if(x_index >= target->x_length) 
        return(X_INDEX_OUT_OF_RANGE);

    index = ((target->length+1)*sizeof(wchar_t))*x_index;

    wcscpy((wchar_t*)(target->data+index), val);
    return(0);
}

int add2DStringArray(vars_t *anker, char *group, char *name, int var_length, int x_length, int y_length)
{
    vars_t *end, *new, *grp;
    int ret, size;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = TWODSTRING;
    new->length = var_length+1;
    new->x_length = x_length;
    new->y_length = y_length;
    size = (((var_length+1)*sizeof(wchar_t))*y_length)*x_length;
    if((new->data = malloc(size)) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    return(0);
}


int edit2DStringArray(vars_t *anker, char *group, char *name, wchar_t *val, int x_index, int y_index)
{
    vars_t *target, *grp;
    int index;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    if(wcslen(val)+1 >= target->length)
        return(VAR_VALUE_TO_LONG);        

    if(x_index >= target->x_length) 
        return(X_INDEX_OUT_OF_RANGE);

    index = (target->y_length*(target->length*sizeof(wchar_t)));
    index = index*(x_index)+((target->length*sizeof(wchar_t))*y_index);

    wcscpy((wchar_t*)(target->data+index), val);
    return(0);
}

int add3DStringArray(vars_t *anker, char *group, char *name, int var_length, int x_length, int y_length, int z_length)
{
    vars_t *end, *new, *grp;
    int ret, size;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = THREEDSTRING;
    new->length = var_length+1;
    new->x_length = x_length;
    new->y_length = y_length;
    new->z_length = z_length;

    size = x_length * y_length * z_length *sizeof(wchar_t);
    size = size*(var_length+1);

    if((new->data = malloc(size)) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    return(0);
}


int edit3DStringArray(vars_t *anker, char *group, char *name, wchar_t *val, int x_index, int y_index, int z_index)
{
    vars_t *target, *grp;
    int index;

    int var_size, sizeofx, sizeofy, sizeofz;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    if(wcslen(val)+1 >= target->length)
        return(VAR_VALUE_TO_LONG);        

    if(x_index >= target->x_length) 
        return(X_INDEX_OUT_OF_RANGE);


    var_size = target->length*sizeof(wchar_t);
    sizeofz = var_size*(target->z_length);
    sizeofy = sizeofz*(target->y_length);
    sizeofx = sizeofy*(target->x_length);

    index = (x_index*sizeofy)+(y_index*sizeofz)+(var_size*z_index);
    printf("x: [%d]\ny: [%d]\nz: [%d]\n\n", x_index, y_index, z_index);

    wcscpy((wchar_t*)(target->data+index), val);
    return(0);
}
