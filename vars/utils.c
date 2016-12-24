#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "utils.h"

int initVarAnker(vars_t **anker)
{
    if(!(*anker = malloc(sizeof(vars_t))))
    {
        return(MEMORY_ALLOC_ERROR);
    }

    if(!((*anker)->name = malloc(1)))
    {
        free(*anker);
        return(MEMORY_ALLOC_ERROR);
    }

    (*anker)->name[0] = '\0';
    (*anker)->type = -1;
    (*anker)->prev = NULL;
    (*anker)->next_lvl = NULL;
    (*anker)->next = NULL;
    return(0);
}

vars_t *goToAnkerEnd(vars_t *anker)
{
    vars_t *hptr = anker;

    while(hptr->next)
        hptr = hptr->next;

    return(hptr);
}

vars_t *isDefined(vars_t *anker, char *name)
{
    vars_t *hptr = anker;

    while(hptr)
    {
        if(strcmp(hptr->name, name) == 0)
        {
            return(hptr);
        }
        hptr  = hptr->next;
    }
    return(false);
}

bool isDefinedBool(vars_t *anker, char *name)
{
    vars_t *hptr = anker;

    while(hptr)
    {
        if(strcmp(hptr->name, name) == 0)
        {
            return(true);
        }
        hptr  = hptr->next;
    }
    return(false);
}

int removeVar(vars_t *anker, char *name)
{
    vars_t *sav = NULL,
           *target = NULL;

    if((target = isDefined(anker, name)) == NULL)
    {
        return(VAR_NOT_DEFINED);
    }
      
    sav = target->prev;   
    sav->next = target->next;

    free(target->name);
    free(target->data);
    free(target);
    return(0);
}

int addNewVar(vars_t *anker, vars_t **new, char *group, char *name)
{
    vars_t *end = NULL,
           *grp = NULL; 
    int use_grp = false;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(VAR_ALREADY_DEFINED);
        }
        if(grp->next_lvl)
            end = goToAnkerEnd(grp->next_lvl);
        else
        {
            use_grp = true;
            end = grp;
        }
    }
    else
    {
        if(isDefined(anker, name))
        {
            return(VAR_ALREADY_DEFINED);
        }
        end = goToAnkerEnd(anker);
    }

    if((*new = malloc(sizeof(vars_t))) == NULL)
    {
        return(MEMORY_ALLOC_ERROR);
    }
    
    if(((*new)->name = malloc(strlen(name)+1)) == NULL)
    {
        free(*new);
        return(MEMORY_ALLOC_ERROR);
    }

    strcpy((*new)->name, name);

    if(use_grp)
        end->next_lvl = *new;
    else
        end->next = *new;
    (*new)->prev = end;
    (*new)->next = NULL;
    return(0);
}

int getVarType(vars_t *anker, char *group, char *name)
{
    vars_t *target = NULL,
           *grp = NULL;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(-1);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(-2);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(-2);
        }
    }

    return(target->type);
}
