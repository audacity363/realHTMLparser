#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

#define open 1
#define closed 2

#define VARIABLEBLOCK 2
#define COMMANDBLOCK 3

#define COMMANDBEGIN_STR "{%"
#define COMMANDEND_STR "%}"
#define VARIABLEBEGIN_STR "{{"
#define VARIABLEEND_STR "}}"

enum {
    COMMANDBEGIN,
    COMMANDEND,
    VARIABLEBEGIN,
    VARIABLEEND,
    VARIABLENAME,
    COMMANDNAME,
    INDEXOPEN,
    INDEXCLOSE,
    INDEXVAL,
    SPACE,
    CHAR,
    STRING,
    EQUALS,
    GREATERTHEN,
    LESSTHEN,

};

struct token_s
{
    char val;
    int type;
    struct token_s *prev;
    struct token_s *next;
};

struct status_s
{
    int lookfor;
    int just_save;
};

typedef struct token_s token_t;
typedef struct status_s status_t;

int addToken(token_t *anker, char token, int type)
{
    token_t *hptr = anker,
            *new;

    while(hptr->next)
    {
        hptr = hptr->next;
    }
    
    if((new = malloc(sizeof(token_t))) == NULL)
    {
        return(false);
    }

    hptr->next = new;
    new->prev = hptr;
    new->next = NULL;

    new->type = type;
    new->val = token;

    return(true);
}

void printTokens(token_t *anker)
{
    token_t *hptr = anker->next;

    while(hptr)
    {
        switch(hptr->type)
        {
            case VARIABLEBEGIN:
                printf("Begin Var\n");
                break;
            case VARIABLEEND:
                printf("End Var\n");
                break;
            case INDEXOPEN:
                printf("Index open\n");
                break;
            case INDEXCLOSE:
                printf("Index close\n");
                break;
            case CHAR:
                printf("Alphanumeric Char\n");
                break;
            case STRING:
                printf("String start/ende\n");
                break;
            case EQUALS:
                printf("gleich\n");
                break;
            case GREATERTHEN:
                printf("groesser als");
                break;
            case LESSTHEN:
                printf("kleiner als");
                break;
            case SPACE:
                printf("Space\n");
                break;
        }
        hptr = hptr->next;
    }
}

void deleteTokens(token_t *anker)
{
    token_t *hptr = anker,
            *prev;

    //Zum ende der Liste gehen
    while(hptr->next)
        hptr = hptr->next;

    prev = hptr->prev;
    while(prev)
    {
        free(hptr);
        hptr = prev;
        prev = hptr->prev;
    }
}

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


int parseCommand(char *begin, char *end)
{
    token_t anker;
    char *curpos = begin;

    anker.next = NULL;
    anker.prev = NULL;

    while(curpos != end)
    {
        switch(curpos[0])
        {
            case '{':
                addToken(&anker, curpos[0], COMMANDBEGIN);
                break;
            case '}':
                addToken(&anker, curpos[0], COMMANDBEGIN);
                break;
            case '%':
                addToken(&anker, curpos[0], COMMANDEND);
                break;
            case '[':
                addToken(&anker, curpos[0], INDEXOPEN);
                break;
            case ']':
                addToken(&anker, curpos[0], INDEXCLOSE);
                break;
            case ' ':
                addToken(&anker, curpos[0], SPACE);
                break;
            case '"':
                addToken(&anker, curpos[0], STRING);
                break;
             case '=':
                addToken(&anker, curpos[0], EQUALS);
                break;
             case '>':
                addToken(&anker, curpos[0], GREATERTHEN);
                break;
             case '<':
                addToken(&anker, curpos[0], LESSTHEN);
                break;
            default:
                addToken(&anker, curpos[0], CHAR);
                break;
        }
        curpos++;
    }
    printTokens(&anker);
    return(0);
}

int parseLine(char *line, status_t *status)
{
    char *inputstr = line,
         *begin,
         *end,
         *prev_end = NULL;
    int len = strlen(line),
        i,
        inblock = false,
        restlength = -1,
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
                parseVariable(begin, end);
            }
            else if(inblock == COMMANDBLOCK)
            {
                parseCommand(begin, end);
            }
            restlength=strlen(end+2);
            prev_end = inputstr = end+2;
        }
        //Text hinter dem letzen Block ausgeben
        else
        {
            printf("kenn ich nicht: [%s]\n", inputstr);
            restlength = restlength - strlen(inputstr);
        }
    }

}

int main()
{
    status_t status;  

    char *inputstr = "{% if test == \"Test Hello\" %}";
    parseLine(inputstr);
}
