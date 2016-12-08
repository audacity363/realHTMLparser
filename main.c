#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "vars.h"
#include "parser.h"
#include "token_handling.h"

vars_t *vars_anker;

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

int initVars(vars_t *anker)
{
    int ret, i, x, y, z;
    float t = 82.62;
    wchar_t strbuff[100];

    if((ret = addGroup(anker, "hallo")) != 0)
    {
        return(1);
    }

    addInteger(anker, "hallo", "test", 677);
    addInteger(anker, NULL, "test1", 67);
    addInteger(anker, "hallo", "test2", 32);

    add1DIntegerArray(anker, "hallo", "array", 10);
    for(i=0; i < 10; i++)
    {

        edit1DIntegerArray(anker, "hallo", "array", i, i);
    }

    add2DIntegerArray(anker, NULL, "2darray", 5, 7);

    z = 0;
    for(i=0; i < 5; i++)
    {
        for(x=0; x < 7; x++)
        {
            edit2DIntegerArray(anker, NULL, "2darray", z, i, x);
            z++;
        }
    }

    i = 0;
    add3DIntegerArray(anker, NULL, "3darray", 5, 7, 9);

    for(x=0; x < 5; x++)
        for(y=0; y < 7; y++)
            for(z=0; z < 9; z++)
            {
                edit3DIntegerArray(anker, NULL, "3darray", i, x, y, z);
                i++;
            }
    if((ret = addString(anker, NULL, "teststr", L"höhöhöhöh", 100)) != 0)
    {
        return(1);
    }

    add1DStringArray(anker, NULL, "string1d", 100, 10);

    for(x=0; x < 10; x++)
    {
        swprintf(strbuff, 100, L"hühühü %d", x);
        edit1DStringArray(anker, NULL, "string1d", strbuff, x);
    }

    add2DStringArray(anker, NULL, "string2d", 100, 5, 7);

    i = 0;
    for(x=0; x < 5; x++)
        for(y=0; y < 7; y++)
        {
            swprintf(strbuff, 100, L"hähähä %d", i);
            if((ret = edit2DStringArray(anker, NULL, "string2d", strbuff, x, y)) != 0)
            {
                return(1);
            }
            i++;
        }

    i=0;
    add3DStringArray(anker, NULL, "string3d",100, 5, 6, 7);
    for(x=0; x < 5; x++)
        for(y=0; y < 6; y++)
            for(z=0; z < 7; z++)
            {
                swprintf(strbuff, 100, L"hä %d", i);
                if((ret = edit3DStringArray(anker, NULL, "string3d", strbuff, x, y, z)) != 0)
                {
                    return(1);
                }
                i++;
            }



    if((ret = addBoolean(anker, NULL, "bool", 1)) != 0)
    {
        return(1);
    }

    if((ret = add1DBooleanArray(anker, NULL, "bool1d", 10)) != 0)
    {
        return(1);
    }

    for(x=0; x < 10; x++)
    {
        if(x % 2)
            edit1DBooleanArray(anker, NULL, "bool1d", 0, x);
        else
            edit1DBooleanArray(anker, NULL, "bool1d", 1, x);
    }


    if((ret = add2DBooleanArray(anker, NULL, "bool2d", 5, 6)) != 0)
    {
        return(1);
    }
    for(x=0; x < 10; x++)
        for(y=0; y < 6; y++)
        {
            if(x % 2)
                edit1DBooleanArray(anker, NULL, "bool2d", 0, x, y);
            else
                edit1DBooleanArray(anker, NULL, "bool2d", 1, x, y);
        }

    addFloat(anker, NULL, "float", 72.62);
    add1DFloatArray(anker, NULL, "float1d", 5);
    for(x=0; x < 5; x++)
    {
        edit1DFloatArray(anker, NULL, "float1d", 72.9276, x);
    }

    return(0);
}

int main()
{
    status_t status;  
    int i, ret;
    vars_t *anker = NULL;

    setlocale(LC_ALL, "");

    if((ret = initVarAnker(&anker)) != 0)
    {
        return(1);
    }

    if(initVars(anker) != 0)
        return(-1);

    vars_anker = anker;
    printAllVars(vars_anker);
#define INPUTSTRS_LENGTH 6

    char *inputstrs[INPUTSTRS_LENGTH] = 
    {
        "{% if bool1d[1] < \"Hello World\" %}",
        "{% for bla == bla %}",
        "{{ Test }} Hello World {{Noch ein Test}}",
        "{% end-for %}",
        "{{balbalbal}}",
        "{% end-if %}"
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
