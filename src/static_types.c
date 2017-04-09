#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "token.h"
#include "variable.h"
#include "macros.h"

#define TRUE_STR L"true"
#define FALSE_STR L"false"

int parseStaticType(Token_Object *start, int *type, void **data)
{
    Token_Object *hptr = start;

    wchar_t *buffer = NULL, *point = NULL;
    int length = -1;

    *type = -1;

    if(hptr->type == QUOTATIONMARKS)
    {
        *type = STRING;
        hptr = hptr->next;
    }

    while(hptr)
    {
        if(hptr->type == QUOTATIONMARKS && *type == STRING)
            break;
        if(length == -1)
        {
            buffer = malloc(SIZEOF_CHAR);
            length = 1;
        }
        else
            buffer = realloc(buffer, (++length)*SIZEOF_WCHAR);
        buffer[length-1] = hptr->val;
        hptr = hptr->next;
    }

    if(*type == STRING)
    {
        *data = (void*)buffer;
        return(0);
    }

    if(wcscmp(buffer, TRUE_STR) == 0)
    {
        *type = BOOL;
        *data = malloc(sizeof(bool));
        *((bool*)(*data)) = true;
    }
    else if(wcscmp(buffer, FALSE_STR) == 0)
    {
        *type = BOOL;
        *data = malloc(sizeof(bool));
        *((bool*)(*data)) = false;
    }
    else if((point = wcschr(buffer, '.')) != NULL)
    {
        *type = FLOAT;
        *data = malloc(sizeof(double));
        *((double*)(*data)) = wcstod(buffer, NULL);
    }
    else
    {
        *type = INTEGER;
        *data = malloc(sizeof(int));
        *((int*)(*data)) = (int)wcstol(buffer, NULL, 10);
    }
    printf("value: [%S]\n", buffer);

    free(buffer);
    
    return(0);
}
