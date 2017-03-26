#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "var_management.h"

int rh4n_errno = 0;

int initAnker(VariableObject **anker)
{
    if((*anker = malloc(SIZEOF_VARIABLEOBJ)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    (*anker)->next_lvl = NULL;
    (*anker)->next = NULL;
    (*anker)->prev = NULL;

    (*anker)->type = -1;
    (*anker)->name = malloc(1);
    *(*anker)->name = '\0';
    
    (*anker)->data = NULL;
    (*anker)->length = 0; 
    memset((*anker)->array_length, sizeof((*anker)->array_length), 0x00);
    (*anker)->size = 0;
    
    (*anker)->flags = 0;
    return(0); 
}

//Goes throu the top level variables and returns the requested group
//If nothing was found it returns NULL
VariableObject *getGroup(VariableObject *anker, char *group)
{
    VariableObject *hptr = anker;

    while(hptr)
    {
        if(NAME_COMPARE(group, hptr) && hptr->type == GROUP)
            return(hptr);

        hptr = hptr->next;
    }

    rh4n_errno = GROUP_NOT_DEFINED;

    return(NULL);
}

bool isDefinedGroup(VariableObject *anker, char *group)
{
    VariableObject *hptr = anker;

    while(hptr)
    {
        if(NAME_COMPARE(group, hptr) && hptr->type == GROUP)
            return(true);

        hptr = hptr->next;
    }

    return(false);

}

VariableObject *getVariable(VariableObject *anker, char *group, char *name)
{
    VariableObject *hptr = NULL;

    if(group)
    {
        if((hptr = getGroup(anker, group)) == NULL)
            return(NULL);
        hptr = hptr->next_lvl;
    }
    else
        hptr = anker;

    while(hptr)
    {
        if(NAME_COMPARE(name, hptr) && hptr->type != GROUP)
            return(hptr);
        hptr = hptr->next;
    }
    
    rh4n_errno = VARIABLE_NOT_DEFINED;
    return(NULL);
}

bool isDefinedVariable(VariableObject *anker, char *group, char *name)
{
    VariableObject *hptr = NULL;

    if(group)
    {
        if((hptr = getGroup(anker, group)) == NULL)
            return(false);
        hptr = hptr->next_lvl;
    }
    else
        hptr = anker;

    while(hptr)
    {
        if(NAME_COMPARE(name, hptr) && hptr->type != GROUP)
            return(false);
        hptr = hptr->next;
    }
    
    return(false);

}

//Adds a new Variable and return the pointer to it
VariableObject *addNewVariable(VariableObject *anker, char *group, char *name, int type)
{

    VariableObject *sav = NULL, *hptr = NULL, *head = NULL;

    if(isDefinedVariable(anker, group, name))
    {
        rh4n_errno = VARIABLE_ALREADY_DEFINED;
        return(NULL);
    }

    if(group)
    {
        if((head = getGroup(anker, group)) == NULL)
            return(NULL);
        head = head->next_lvl;
    }
    else
        head = anker;

    //Go to the end of the current fork
    for(; head->next; head = head->next);

    if((sav = malloc(SIZEOF_VARIABLEOBJ)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(NULL);
    }

    sav->prev = head;
    sav->next_lvl = NULL;
    head->next = sav;

    sav->type = type;
    
    if((sav->name = malloc(strlen(name)+1)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        free(sav);
        head->next = NULL;

        return(NULL);
    }
    strcpy(sav->name, name);

    sav->data = NULL;
    sav->length = 0; memset(sav->array_length, sizeof(sav->array_length), 0x00);
    sav->size = 0;
    
    sav->flags = 0;

    return(sav);
}

int addNewGroup(VariableObject *anker, char *group)
{
    VariableObject *hptr = anker, *sav = NULL;  
    if(isDefinedGroup(anker, group))
    {
        rh4n_errno = GROUP_ALREADY_DEFINED;
        return(-1);
    }

    //Go to the end of the current fork
    for(; hptr->next; hptr = hptr->next);

    if((sav = malloc(SIZEOF_VARIABLEOBJ)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    sav->prev = hptr;
    sav->next_lvl = NULL;
    hptr->next = sav;

    sav->type = GROUP;
    
    if((sav->name = malloc(strlen(group)+1)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        free(sav);
        hptr->next = NULL;

        return(-1);
    }
    strcpy(sav->name, group);

    sav->data = NULL;
    sav->length = 0; memset(sav->array_length, sizeof(sav->array_length), 0x00);
    sav->size = 0;
    
    sav->flags = 0;

    //Init group anker
    if((sav->next_lvl = malloc(SIZEOF_VARIABLEOBJ)) == NULL)
    {
        rh4n_errno = MEMORY_ERROR;
        return(-1);
    }

    sav->next_lvl->prev = sav;

    sav = sav->next_lvl;
    
    sav->next_lvl = NULL;
    sav->next = NULL;

    sav->type = -1;
    sav->name = malloc(1);
    *sav->name = '\0';
    
    sav->data = NULL;
    sav->length = 0; memset(sav->array_length, sizeof(sav->array_length), 0x00);
    sav->size = 0;
    
    sav->flags = 0;
    return(0);
}
