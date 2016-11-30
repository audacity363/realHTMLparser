#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "token_handling.h"

//Gibt den Typ des Blocker zurueck. Wenn nichts gefunden false 
//Variablenblock = VARIABLENBLOCK
//Commandblock = COMMANDBLOCK
int searchBlockBegin(char *str, char **pos)
{
    char *begin;

    if((begin = strstr(str, VARIABLEBEGIN_STR)) != NULL)
    {
        *pos = begin;
        return(VARIABLEBLOCK);
    }
    else if((begin = strstr(str, COMMANDBEGIN_STR)) != NULL)
    {
        *pos = begin;
        return(COMMANDBLOCK);
    }
    return(false);
}

int searchBlockEnd(char *str, char **pos, int type)
{
    char *end;

    if(type == VARIABLEBLOCK)
    {
        if((end = strstr(str, VARIABLEEND_STR)) != NULL)
        {
            *pos = end;
            return(true);
        }
        return(false);
    }
    else if(type == COMMANDBLOCK)
    {
        if((end = strstr(str, COMMANDEND_STR)) != NULL)
        {
            *pos = end;
            return(true);
        }
        return(false);
    }
    return(false);
}

int saveLine(char *line, status_t *stat)
{
    if(stat->sizeof_sav_buff == 0)
    {
        stat->save_buff = malloc(sizeof(char*));
        stat->save_buff[0] = malloc(strlen(line)+1);
        strcpy(stat->save_buff[0], line);
        stat->sizeof_sav_buff = 1;
    }
    else
    {
        stat->sizeof_sav_buff++;
        stat->save_buff = realloc(stat->save_buff, sizeof(char*)*stat->sizeof_sav_buff);
        stat->save_buff[stat->sizeof_sav_buff-1] = malloc(strlen(line)+1);
        strcpy(stat->save_buff[stat->sizeof_sav_buff-1], line);
    }
    return(0);
}

void freeLineBuff(status_t *stat)
{
    int i;

    for(i=stat->sizeof_sav_buff-1; i > 0; i--)
    {
        free(stat->save_buff[i]);
    }
    free(stat->save_buff);
    stat->save_buff = NULL;
    stat->sizeof_sav_buff = 0;
}

int parseLine(char *line, status_t *status)
{
    char *inputstr = line,
         *begin,
         *end,
         *prev_end = NULL,
         *backup_line = line;
    int len = strlen(line),
        i,
        inblock = false,
        restlength = strlen(line),
        between_len;

    printf("parse: [%s]\n", inputstr);
    while(restlength != 0) 
    { 
        if((inblock = searchBlockBegin(inputstr, &begin)) != false)
        {
            //Text zwischen den Bloecken ausgeben
            if(prev_end)
            {
                between_len = begin - prev_end;
                //printf("between_len: [%d]\n", between_len);
                printf("Kenn ich nicht [%.*s]\n", between_len, prev_end);
            }
            //Text vor dem ersten Block ausgeben
            if(!prev_end)
            {
                between_len = begin - inputstr;
                printf("Kenn ich nicht [%.*s]\n", between_len, inputstr);
            }
            if(searchBlockEnd(begin, &end, inblock) == false)
            {
                fprintf(stderr, "Syntax Error Missing Endbracked\n");
                break;
            }
            else if(inblock == VARIABLEBLOCK)
            {
                if(status->just_save)
                {
                    saveLine(line, status);
                    break;
                }
                parseVariable(begin, end);
            }
            else if(inblock == COMMANDBLOCK)
            {
                if(parseCommand(begin, end, status) == JUSTSAVE)
                {
                    saveLine(backup_line, status);
                    break;
                }
            }
            restlength=strlen(end+2);
            prev_end = inputstr = end+2;
        }
        //Text hinter dem letzen Block ausgeben
        else
        {
            printf("kenn ich nicht: [%s]\n", inputstr);
            restlength = restlength - strlen(inputstr);
            if(status->just_save)
                saveLine(line, status);
        }
    }

}

int main()
{
    status_t status;  
    int i;

#define INPUTSTRS_LENGTH 6

    char *inputstrs[INPUTSTRS_LENGTH] = 
    {
        "{% for test == \"Hello World\" %}",
        "{% for bla == bla %}",
        "{{ Test }} Hello World {{Noch ein Test}}",
        "{% end-for %}",
        "{{balbalbal}}",
        "{% end-for %}"
    };

    status.in_for = 0;
    status.in_if = 0;
    status.save_buff= NULL;
    status.sizeof_sav_buff = 0;

    for(i=0; i < INPUTSTRS_LENGTH; i++)
    {
        parseLine(inputstrs[i], &status);
    }

    for(i=0; i < status.sizeof_sav_buff; i++)
    {
        printf("%s\n", status.save_buff[i]);
    }

    freeLineBuff(&status);
}
