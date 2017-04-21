#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "parser.h"
#include "token.h"
#include "parser_utils.h"
#include "variable.h"
#include "for.h"


int initForVariables(VariableObject *anker, LoopProperties *for_status)
{
    VariableObject *hptr = NULL;
    int i = 0;
    //Check if the internal variable "loop.i" is already definied (probably nested for loops)
    if(isDefinedVariable(anker, "loop", "i") == true)
    {
        setInteger(anker, "loop", "i", 0);
    }
    else
    {
        if(isDefinedGroup(anker, "loop") == false)
        {
            addNewGroup(anker, "loop");
        }
        newInteger(anker, "loop", "i");
        setInteger(anker, "loop", "i", 0);
    }

    for(hptr=anker; hptr->next != NULL; hptr=hptr->next);

    for_status->old_type = malloc(for_status->length_of_vars*sizeof(int));
    for_status->data_start = malloc(for_status->length_of_vars*sizeof(void*));
    memset(for_status->old_type, 0x00, for_status->length_of_vars*sizeof(int));

    for(i=0; i < for_status->length_of_vars;i++)
    {
        for_status->old_type[i] = for_status->vars[i].target.type;
        for_status->data_start[i] = for_status->vars[i].target.data;
        translateVarTypeWithIndex(for_status->vars[i].target.type, &for_status->vars[i].target.type);

        for_status->vars[i].target.prev = hptr;
        hptr->next = &for_status->vars[i].target;
        hptr = hptr->next;
    }

    return(0);
}

int nextForVariable(LoopProperties *for_status)
{
    int i = 0, data_offset = 0;

    for(; i < for_status->length_of_vars; i++)
    {
        data_offset = calculateOffset(for_status->old_type[i], for_status->index, 
            for_status->vars[i].target.array_length, for_status->vars[i].target.length);
        for_status->vars[i].target.data = for_status->data_start[i]+data_offset;
    }
    setInteger(var_anker, "loop", "i", for_status->index);
    return(0);
}


//TODO: Free the attributes and targets
int cleanupForVariables(LoopProperties *for_status)
{
    int i = 0;
    VariableObject *hptr = NULL;

    for(; i < for_status->length_of_vars; i++)
    {
        hptr = &for_status->vars[i].target;
        if(hptr->next)
        {
            hptr->next->prev = hptr->prev;
        }
        hptr->prev->next = hptr->next;
        hptr->next = NULL;
        hptr->prev = NULL;
    }
    return(0);
}
