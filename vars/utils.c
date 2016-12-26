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

/*
 * Creates a new variable of the type given in type. Just normal vars are
 * supported. No arrays
 */
int addVariableBasedOnType(vars_t *anker, int type, char *name, void *val)
{
    int ret = 0;

    switch(type)
    {
        case STRING:
            ret = addString(anker, NULL, name, (wchar_t*)val, wcslen((wchar_t*)val));
            break;
        case INTEGER:
            ret = addInteger(anker, NULL, name, *((int*)val));
            break;
        case FLOAT:
            ret = addFloat(anker, NULL, name, *((double*)val));
            break;
        case BOOL:
            ret = addBoolean(anker, NULL, name, *((bool*)val));
            break;
    }
    return(ret);
}

int copyVariable(vars_t *anker, vars_t *target, char *group, char *name)
{
    return(copyVariableNewName(anker, target, name, NULL, name, NULL));
}

int copyVariableNewName(vars_t *anker, vars_t *target_ank, char *group, char *name, char *new_grp, char *new_name)
{
    int ret = 0;
    vars_t *target = NULL,
           *grp = NULL;

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

    //TODO: Add missing variable types
    switch(target->type)
    {
        case GROUP:
            ret = copyGroupNewName(anker, target_ank, name, new_name, 0,0,0);
            break;
        case INTEGER:
            ret = addInteger(target_ank, new_grp, new_name,
                    *((int*)target->data));
            break;
        case FLOAT:
            ret = addFloat(target_ank, new_grp, new_name,
                    *((double*)target->data));
            break;
        case BOOL:
            ret = addBoolean(target_ank, new_grp, new_name,
                    *((bool*)target->data));
            break;
        case ONEDINTEGER:
            ret = add1DIntegerArray(target_ank, new_grp, new_name, 
                    target->x_length);
            if(ret != 0)
                return(ret);
            editFull1DIntegerArray(target_ank, new_grp, new_name,
                    target->data);
            break;
        case TWODINTEGER:
            ret = add2DIntegerArray(target_ank, new_grp, new_name,
                    target->x_length, target->y_length);
            if(ret != 0)
                return(ret);
            editFull2DIntegerArray(target_ank, new_grp, new_name,
                    target->data);
            break;
        case THREEDINTEGER:
            ret = add3DIntegerArray(target_ank, new_grp, new_name,
                    target->x_length, target->y_length, target->z_length);
            if(ret != 0)
                return(ret);
            editFull3DIntegerArray(target_ank, new_grp, new_name,
                    target->data);
            break;
    }
    return(ret);
}

int copyGroupNewName(vars_t *anker, vars_t *target, char *name, char *new_name)
{
    vars_t *grp = NULL, *hptr = NULL;

    addGroup(target, new_name, 0,0,0);

    grp = isDefined(anker, name);

    hptr = grp->next_lvl;

    while(hptr)
    {
        copyVariableNewName(anker, target, name, hptr->name, new_name, hptr->name);
        hptr = hptr->next;
    }
}

/*
 * Copys a variable or a complete group but uses just the given index.
 * When a group is given then every variable under this group gets copied with the given index
 * TODO: There are a lot of arguments. Should I use a struct only for this purpose??
 */
int copyVariableNewNameWithIndex(vars_t *anker, vars_t *target_ank,
        char *group, char *name, char *new_grp, char *new_name,
        int index_type, int index_array[3])
{
   int var_type = 0; 

   if((var_type = getVarType(anker, group, name)) < 0)
   {
        return(var_type);
   }

    //Go recursive through the group and copy the variables
    if(var_type == GROUP)
    {
        copyGroupNewNameWithIndex(anker, target_ank,
        name, new_name, index_type, index_array);
        return(0);
    }

   //First check if the variable type is not a simple type
   if((var_type == STRING  || 
       var_type == BOOL    ||
       var_type == FLOAT   ||
       var_type == INTEGER))
   {
        return(INDEX_IS_NOT_SUPPORTED);
   }

   //Check if the var type is compatible with the index type
   else if(index_type == 3 && (var_type == ONEDINTEGER ||
                               var_type == ONEDSTRING  ||
                               var_type == ONEDFLOAT   ||
                               var_type == ONEDBOOL    ||
                               var_type == TWODINTEGER ||
                               var_type == TWODSTRING  ||
                               var_type == TWODFLOAT   ||
                               var_type == TWODBOOL))
    {
        return(INDEX_IS_NOT_SUPPORTED);
    }
    else if(index_type == 2 && (var_type == ONEDINTEGER ||
                                var_type == ONEDSTRING  ||
                                var_type == ONEDFLOAT   ||
                                var_type == ONEDBOOL)) 
    {
        return(INDEX_IS_NOT_SUPPORTED);
    }

    //Call the corresponding for the index type and variable type combo
    //TODO: Write the rest of the functions and append the list here
    if(index_type == 1 && var_type == ONEDINTEGER)
        return(createNewVarFrom1DIntegerArray(anker, target_ank,
                                  group, name, new_grp,
                                  new_name, index_array[0]));

}

int copyGroupNewNameWithIndex(vars_t *anker, vars_t *target_ank,
        char *name, char *new_name,
        int index_type, int index_array[3])
{
    vars_t *grp = NULL, *hptr = NULL;
    int ret = 0;

    if(!(grp = isDefined(anker, name)))
    {
        return(GRP_NOT_DEFINED);
    }

    addGroup(target_ank, new_name, 0,0,0);

    hptr = grp->next_lvl;
    while(hptr)
    {

        if((ret = copyVariableNewNameWithIndex(anker, target_ank,
        name, hptr->name, new_name, hptr->name,
        index_type, index_array)) != 0)
            return(0);
        hptr = hptr->next;
    }
}
