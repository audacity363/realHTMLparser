#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "token_handling.h"
#include "command_parsing.h"
#include "if.h"
#include "for.h"

int printVars_handling(token_t*, status_t*);

#define FUNCTION_DIC_LENGTH 5 
char function_dic_c[FUNCTION_DIC_LENGTH][50] = {
        "if",
        "end-if",
        "for",
        "end-for",
        "printVars"
    };

int (*function_dic_v[FUNCTION_DIC_LENGTH])(token_t *anker, status_t *stat) = {
        if_handling,
        end_if_handling,
        for_handling,
        end_for_handling,
        printVars_handling
    };

int printVars_handling(token_t *anker, status_t *stat)
{
    printf("Ich gebe alle Variablen aus\n");
    return(0);
}

int (*getCommandFunction(char *cmd_name))(token_t *anker, status_t *stat)
{
    int i;

    for(i=0; i <= FUNCTION_DIC_LENGTH; i++)
    {
        if(strcmp(function_dic_c[i], cmd_name) == 0)
            return(function_dic_v[i]);
    }
    return(NULL);
}

int getCommandLength(token_t *anker, int *end_cmd_offset)
{
    token_t *hptr = anker->next;
    int length = 0,
        found_start = false,
        offset = 0;

    while(hptr)
    {
        if(hptr->type == SPACE && found_start == true)
        {
            break;
        }
        else if(hptr->type == CLINGTO && found_start == true)
        {
            break;
        }
        else if(hptr->type == CLINGTO && found_start == false)
        {
            fprintf(stderr, "Unexpected Token\n");
            return(-1);
        }
        else if(hptr->type == CLAMPS)
        {
            fprintf(stderr, "Unexpected Token\n");
            return(-2);
        }
        else if(hptr->type == CHAR)
        {
            found_start = true;
            length++;
        }
        hptr = hptr->next;
        offset++;
    }
    *end_cmd_offset = offset;
    return(length);
}

int getCommandName(token_t *anker, char *cmd_name)
{
    token_t *hptr = anker->next;
    int length = 0,
        found_start = false;

    while(hptr)
    {
        if(hptr->type == SPACE && found_start == true)
        {
            break;
        }
        else if(hptr->type == CLINGTO && found_start == true)
        {
            break;
        }
        else if(hptr->type == CLINGTO && found_start == false)
        {
            fprintf(stderr, "Unexpected Token\n");
            return(-1);
        }
        else if(hptr->type == CLAMPS)
        {
            fprintf(stderr, "Unexpected Token\n");
            return(-2);
        }
        else if(hptr->type == CHAR)
        {
            cmd_name[length] = hptr->val;
            found_start = true;
            length++;
        }
        hptr = hptr->next;
    }

    cmd_name[length] = '\0'; 
    return(0);
}


int lineToTokens(token_t *anker, char *begin, char *end)
{
    char *curpos = begin;

    while(curpos != end)
    {
        switch(curpos[0])
        {
            case '{':
                addToken(anker, curpos[0], BLOCKSTART);
                break;
            case '%':
                addToken(anker, curpos[0], CMDSTARTEND);
                break;
            case '}':
                addToken(anker, curpos[0], BLOCKEND);
                break;
            case '[':
                addToken(anker, curpos[0], INDEXOPEN);
                break;
            case ']':
                addToken(anker, curpos[0], INDEXCLOSE);
                break;
            case ' ':
                addToken(anker, curpos[0], SPACE);
                break;
            case '"':
                addToken(anker, curpos[0], STRING);
                break;
            case '=':
                addToken(anker, curpos[0], EQUALS);
                break;
            case '>':
                addToken(anker, curpos[0], GREATERTHEN);
                break;
            case '<':
                addToken(anker, curpos[0], LESSTHEN);
                break;
            case '(':
                addToken(anker, curpos[0], CLINGTO);
                break;
            case ')':
                addToken(anker, curpos[0], CLAMPS);
                break;
            default:
                addToken(anker, curpos[0], CHAR);
                break;
        }
        curpos++;
    }
    return(0);
}


int parseCommand(char *begin, char *end, status_t *stat)
{
    token_t anker, *offset;
    char *curpos = begin,
         *cmd_name;

    int cmd_name_length, end_cmd_offset, ret;
    int (*cmd_func)(token_t *anker, status_t *stat);

    anker.next = NULL;
    anker.prev = NULL;

    lineToTokens(&anker, begin, end);

    cmd_name_length = getCommandLength(&anker, &end_cmd_offset);

    if((cmd_name = malloc(cmd_name_length+1)) == NULL)
    {
        fprintf(stderr, "Memalloc for cmd name failed\n");
        return(-1);
    }

    getCommandName(&anker, cmd_name);

    if((cmd_func = getCommandFunction(cmd_name)) == NULL)
    {
        free(cmd_name);
        fprintf(stderr, "Unkown function\n");
        return(-2);
    }

    if((offset = jumpToOffset(&anker, end_cmd_offset)) == NULL)
    {
        free(cmd_name);
        fprintf(stderr, "Offset to high\n");
        return(-3);
    }

    ret = cmd_func(offset, stat);

    free(cmd_name);

    return(ret);
}
