#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "utils.h"

int addGroup(vars_t *anker, char *name)
{
    vars_t *end, *new;

    if(isDefined(anker, name))
    {
        return(GRP_ALREADY_DEFINED);
    }

    end = goToAnkerEnd(anker);

    if((new = malloc(sizeof(vars_t))) == NULL)
    {
        return(MEMORY_ALLOC_ERROR);
    }
    
    if((new->name = malloc(strlen(name)+1)) == NULL)
    {
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    strcpy(new->name, name);
    new->type = GROUP;

    end->next = new;
    new->prev = end;
    new->next = NULL;


}

int addInteger(vars_t *anker, char *group, char *name, int val)
{
    vars_t *end, *new, *grp;
    int ret;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = INTEGER;
    if((new->data = malloc(sizeof(int))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    *((int*)new->data) = val;
    return(0);
}

int add1DIntegerArray(vars_t *anker, char *group, char *name, int length)
{
    vars_t *end, *new, *grp;
    int ret;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = ONEDINTEGER;
    new->x_length = length;
    if((new->data = malloc(sizeof(int)*length)) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }
    return(0);
}

int edit1DIntegerArray(vars_t *anker, char *group, char *name, int val, int x_index)
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

    ((int*)target->data)[x_index] = val;
    return(0);
}

int add2DIntegerArray(vars_t *anker, char *group, char *name, int x_length, int y_length)
{
    vars_t *end, *new, *grp;
    int ret;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = TWODINTEGER;
    new->x_length = x_length;
    new->y_length = y_length;
    if((new->data = malloc((x_length*y_length)*sizeof(int))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }
    return(0);

}

int edit2DIntegerArray(vars_t *anker, char *group, char *name, int val, int x_index, int y_index)
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
        return(X_INDEX_OUT_OF_RANGE);
    if(y_index >= target->y_length)
        return(Y_INDEX_OUT_OF_RANGE);


    ((int*)target->data)[target->x_length*x_index+y_index] = val;
    return(0);
}
