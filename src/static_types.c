#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "token.h"
#include "static_types.h"

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
            buffer = malloc(SIZEOF_WCHAR);
            length = 1;
        }
        else
            buffer = realloc(buffer, (++length)*SIZEOF_WCHAR);
        buffer[length-1] = hptr->val;
        hptr = hptr->next;
    }

    if(*type == STRING)
    {
        buffer = realloc(buffer, (++length)*SIZEOF_WCHAR);
        buffer[length-1] = L'\0';
        *data = (void*)buffer;
        return(0);
    }

    /*if(wcscmp(buffer, TRUE_STR) == 0)
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
    printf("value: [%S]\n", buffer);*/
    parseStaticTypeWString(buffer, type, data);

    free(buffer);
    
    return(0);
}

int parseStaticTypeString(char *string, int *type, void **data)
{
    wchar_t *w_buffer = NULL;

    w_buffer = malloc((strlen(string)+1)*SIZEOF_WCHAR);
    mbstowcs(w_buffer, string, strlen(string)+1);
    free(w_buffer);
}

int parseStaticTypeWString(wchar_t *string, int *type, void **data)
{
    wchar_t *point = NULL;
    if(*string == '"') 
    {
        *type == STRING;
        //buffer = realloc(buffer, (strlen(string)-1)*SIZEOF_WCHAR);
        //buffer[strlen(string)-1] = L'\0';
        *data = (void*)(string+1);
        return(0);
    }

    if(wcscmp(string, TRUE_STR) == 0)
    {
        *type = BOOL;
        *data = malloc(sizeof(bool));
        *((bool*)(*data)) = true;
    }
    else if(wcscmp(string, FALSE_STR) == 0)
    {
        *type = BOOL;
        *data = malloc(sizeof(bool));
        *((bool*)(*data)) = false;
    }
    else if((point = wcschr(string, '.')) != NULL)
    {
        *type = FLOAT;
        *data = malloc(sizeof(double));
        *((double*)(*data)) = wcstod(string, NULL);
    }
    else
    {
        *type = INTEGER;
        *data = malloc(sizeof(int));
        *((int*)(*data)) = (int)wcstol(string, NULL, 10);
    }   
}
