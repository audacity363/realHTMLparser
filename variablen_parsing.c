#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "token_handling.h"

int getVarnameLength(token_t *anker)
{
    token_t *hptr = anker->next;
    int length = 0;

    while(hptr)
    {
        if(hptr->type == INDEXOPEN)
        {
            break;
        }
        if(hptr->type == CHAR)
        {
            length++;
        }
        hptr = hptr->next;
    }

    return(length);
}

void getVarname(token_t *anker, char *buff)
{
    token_t *hptr = anker->next;
    int length = 0;

    while(hptr)
    {
        if(hptr->type == INDEXOPEN)
        {
            break;
        }
        if(hptr->type == CHAR)
        {
            buff[length++] = hptr->val;
        }
        hptr = hptr->next;
    }
    buff[length] = '\0';
}

//Sucht den Index raus. 
//Returnwerte: false = kein Index gefunden
//                 1 = 1 Index gefunden
//                 2 = 2 Index gefunden
//                 3 = 3 Index gefunden
//Die gefundenn werden in das Array index_num geschrieben
int getIndex(token_t *anker, int *index_num)
{
    int index_type = 0,
        in_brackets = false,
        closed_found = false,
        i = 0;

    token_t *hptr = anker->next;
    char c_index[4];

    while(hptr)
    {
        if(hptr->type == INDEXOPEN && in_brackets == true)
        {
            fprintf(stderr, "Syntax Error. Missing \"]\"\n");
            return(-1);
        }
        else if(hptr->type == INDEXOPEN && in_brackets == false)
        {
            in_brackets = true;
            index_type++;
        }
        else if(hptr->type == CHAR && in_brackets == true)
        {
            c_index[i++] = hptr->val; 
        }
        else if(hptr->type == INDEXCLOSE && in_brackets == false)
        {
            fprintf(stderr, "Syntax Error. Missing \"[\"\n");
            return(-2);
        }
        else if(hptr->type == INDEXCLOSE && in_brackets == true)
        {
            in_brackets = false;
            c_index[i] = '\0';
            if((index_num[0] = atoi(c_index)) == 0 && c_index[0] != '0')
            {
                fprintf(stderr, "Conv Error\n");
                return(-3);
            }
        } 
        hptr = hptr->next;
    }
    return(index_type);
}

int parseVariable(char *begin, char *end)
{
    char *curpos = begin,
         *variablename;
    token_t anker;
    int varname_length, 
        index_type,
        index_num[3] = {-1, -1, -1},
        ret;

    anker.next = NULL;
    anker.prev = NULL;

    while(curpos != end)
    {
        switch(curpos[0])
        {
            case '{':
                addToken(&anker, curpos[0], VARIABLEBEGIN);
                break;
            case '[':
                addToken(&anker, curpos[0], INDEXOPEN);
                break;
            case ']':
                addToken(&anker, curpos[0], INDEXCLOSE);
                break;
            case '}':
                addToken(&anker, curpos[0], VARIABLEEND);
                break;
            case ' ':
                addToken(&anker, curpos[0], SPACE);
                break;
            default:
                addToken(&anker, curpos[0], CHAR);
                break;
                
        }
        curpos++;
    }
    printTokens(&anker);
    if((varname_length = getVarnameLength(&anker)) == 0) 
    {
        fprintf(stderr, "Keine Variable gefunden\n");
        return(-1);
    }
    varname_length++;
    if((variablename = malloc(varname_length)) == NULL)
    {
        return(-2);
    }

    getVarname(&anker, variablename);
    free(variablename);

    deleteTokens(&anker);
    printf("------------------------------------------------------\n");
    return(0);
}
