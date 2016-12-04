#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "utils.h"

int addBoolean(vars_t *anker, char *group, char *name, bool val)
{
    vars_t *end, *new, *grp;
    int ret;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = BOOL;
    if((new->data = malloc(sizeof(bool))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    *((bool*)new->data) = val;
    return(0);
}

int add1DBooleanArray(vars_t *anker, char *group, char *name, int length)
{
    vars_t *end, *new, *grp;
    int ret;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = ONEDBOOL;
    new->x_length = length;
    if((new->data = malloc(sizeof(bool)*length)) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }
    return(0);
}

int edit1DBooleanArray(vars_t *anker, char *group, char *name, bool val, int x_index)
{
    vars_t *target, *grp;

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

    if(x_index >= target->x_length)
    {
        return(X_INDEX_OUT_OF_RANGE);
    }

    ((bool*)target->data)[x_index] = val;
    return(0);
}

int add2DBooleanArray(vars_t *anker, char *group, char *name, int x_length, int y_length)
{
    vars_t *end, *new, *grp;
    int ret;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = TWODBOOL;
    new->x_length = x_length;
    new->y_length = y_length;
    if((new->data = malloc((x_length*y_length)*sizeof(bool))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }
    return(0);

}

int edit2DBooleanArray(vars_t *anker, char *group, char *name, bool val, int x_index, int y_index)
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

    if(x_index >= target->x_length)
        return(X_INDEX_OUT_OF_RANGE);
    if(y_index >= target->y_length)
        return(Y_INDEX_OUT_OF_RANGE);


    index = (x_index*target->y_length)+y_index;
    ((bool*)target->data)[index] = val;
    return(0);
}

int add3DBooleanArray(vars_t *anker, char *group, char *name, int x_length, int y_length, int z_length)
{
    vars_t *end, *new, *grp;
    int ret;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = THREEDBOOL;
    new->x_length = x_length;
    new->y_length = y_length;
    new->z_length = z_length;
    if((new->data = malloc(((x_length*y_length)*z_length)*sizeof(bool))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }
    return(0);

}

int edit3DBoolArray(vars_t *anker, char *group, char *name, bool val, int x_index, int y_index, int z_index)
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

    if(x_index >= target->x_length)
        return(X_INDEX_OUT_OF_RANGE);
    if(y_index >= target->y_length)
        return(Y_INDEX_OUT_OF_RANGE);
    if(z_index >= target->z_length)
        return(Z_INDEX_OUT_OF_RANGE);

    index = (z_index*target->x_length * target->y_length);
    index += (y_index*target->x_length) + x_index;
    ((bool*)target->data)[index] = val;
    return(0);
}
