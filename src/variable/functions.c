#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "variable.h"

int len_func(VariableObject *target)
{
    if(target->type == FLOAT || target->type == BOOL || target->type == INTEGER)
        return(UNSUPPOERTED_TYPE);
  
    if(!ISSET_FLAG(target->type, RH4N_FLG_COPY))
    {
        free(target->data);
        target->data = malloc(sizeof(int));
    }
    
    if(target->type == STRING)
        *((int*)target->data) = target->length;
    else
        *((int*)target->data) = target->array_length[0];

    target->type = INTEGER;
    return(0);
}
