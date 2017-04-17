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

int parseMultipleVars(Token_Object *start, LoopProperties *for_status);
int parseSingleVar(Token_Object *start, LoopProperties *for_status);
Token_Object *jumpOverIn(Token_Object *start, int set_null);


int parseForHead(Token_Object *start, LoopProperties *for_status)
{
    //Multiple vars were given
    if(start->type ==  OPENBRACKET)
    {
        parseMultipleVars(start, for_status);
    }
    else
    {
        parseSingleVar(start, for_status);
    }
    return(0);

}

int parseMultipleVars(Token_Object *start, LoopProperties *for_status)
{
    char **names = NULL;

    int *name_length = NULL, length = -1, i = 0, length_of_entries = -1;

    Token_Object *hptr = start->next, *sav = NULL, **entries = NULL, *end = NULL;

    names = malloc(sizeof(char*));
    names[0] = NULL;

    name_length = malloc(sizeof(int));
    name_length[0] = -1;

    length = 1;

    //save the names in a two d char array
    while(hptr)
    {
        if(hptr->type == COMMA)
        {
            names[length-1] = realloc(names[length-1], (++(name_length[length-1]))*SIZEOF_CHAR);
            names[length-1][(name_length[length-1])-1] = '\0';

            names = realloc(names, (++length)*sizeof(char*));
            name_length = realloc(name_length, (length)*sizeof(int));
            names[length-1] = NULL;
            name_length[length-1] = -1;
        }
        else if(hptr->type == CLOSEBRACKET)
        {
            break;
        }
        else if(hptr->type != CHAR && hptr->type != SPACE)
        {
            //TODO: Free all names
            if(name_length[length-1] != -1)
                free(names);
            PRINT_SYNTAX_ERROR(hptr);
            return(-1);
        }
        else if(hptr->type == SPACE)
        {
            hptr = hptr->next;
            continue;
        }
        else if(CHECK_UNICODE_CHAR(hptr->val))
        {
            //TODO: Free all names
            if(name_length[length-1] != -1)
                free(names);
            PRINT_SYNTAX_ERROR(hptr);
            return(-1);
        }
        else
        {
            if(name_length[length-1] == -1)
            {
                names[length-1] = malloc(SIZEOF_CHAR);
                name_length[length-1] = 1;
            }
            else
            {
                names[length-1] = realloc(names[length-1], (++(name_length[length-1]))*SIZEOF_CHAR);
            }
            names[length-1][(name_length[length-1])-1] = (char)hptr->val;
        }
        hptr = hptr->next;
    }
    names[length-1] = realloc(names[length-1], (++(name_length[length-1]))*SIZEOF_CHAR);
    names[length-1][(name_length[length-1])-1] = '\0';

    for(i=0; i < length; i++)
    {
        printf("Name of for var: No. %d [%s]\n", i, names[i]);
    }

    sav = jumpOverIn(hptr->next, 0);
    hptr->next = NULL;
    cleanTokenList(start);
    
    //Jump to the end of the command
    i=1;
    end = sav->next;
    while(end->next)
    {
        if(end->next->type == OPENBRACKET)
            i++;
        else if(end->next->type == CLOSEBRACKET)
            i--;
        if(i == 0)
            break;

        end = end->next;
    }
    cleanTokenList(end->next);
    end->next = NULL;

    printfromTree(sav);
    printf("\n");
    if(sav->type != OPENBRACKET)
    {
        PRINT_SYNTAX_ERROR(sav);
        cleanTokenList(sav);
        for(i=0; i < length; i++)
        {
            free(names[i]);
        }
        free(names);
        free(name_length);
        return(-1);
    }
    hptr = sav->next;

    //Split the given parms into a array
    entries = malloc(sizeof(Token_Object*)); length_of_entries = 1;
    entries[0] = hptr;

    while(hptr->next)
    {
        if(hptr->next->type == COMMA)
        {
            sav = hptr->next->next;
            entries = realloc(entries, sizeof(Token_Object*)*(++length_of_entries));
            entries[length_of_entries-1] = sav;

            hptr->next = NULL;
            hptr = sav;
        }
        hptr = hptr->next;
    }

    if(length != length_of_entries)
    {
        fprintf(stderr, "FOR: parms does not match Expected [%d] given [%d]\n", length, length_of_entries);
        return(-1);
    }

    for(i=0; i < length_of_entries; i++)
    {
        if(for_status->length_of_vars == -1)
        {
            for_status->vars = malloc(sizeof(VariableParseData));
            memset(&for_status->vars[0], 0x00, sizeof(VariableParseData));
            for_status->vars[0].number_of_attributes = -1;
            for_status->length_of_vars = 1;
        }
        else
        {
            for_status->vars =  realloc(for_status->vars, 
                (++for_status->length_of_vars)*sizeof(VariableParseData));
            memset(&for_status->vars[for_status->length_of_vars-1], 0x00, sizeof(VariableParseData));
            for_status->vars[for_status->length_of_vars-1].number_of_attributes = -1;
        }

        if(getVariableAttributes(entries[i], &for_status->vars[for_status->length_of_vars-1]) < 0)
        {
            fprintf(stderr, "Error in getVariableAttributes()\n");
            free(entries);
            return(-1);
        }
        printAttributes(&for_status->vars[for_status->length_of_vars-1]);

        if(execAttributes(&for_status->vars[for_status->length_of_vars-1]) < 0)
        {
            return(-1);
        }
        
        free(for_status->vars[for_status->length_of_vars-1].target.name);
        for_status->vars[for_status->length_of_vars-1].target.name = malloc((strlen(names[i])+1)*SIZEOF_CHAR);
        strcpy(for_status->vars[for_status->length_of_vars-1].target.name, names[i]);

        printVarPtr(&for_status->vars[for_status->length_of_vars-1].target, PRINT_MODE_FORMAT, stdout);
        printf("\n");
    }

    for(i=0; i < length; i++)
    {
        free(names[i]);
    }

    free(names);
    free(name_length);
    return(0);

}

int parseSingleVar(Token_Object *start, LoopProperties *for_status)
{
    char *name = NULL;

    int name_length = -1;

    Token_Object *hptr = start;

    while(hptr)
    {
        if(hptr->type != CHAR && hptr->type != SPACE)
        {
            if(name_length != -1)
                free(name);
            PRINT_SYNTAX_ERROR(hptr);
            return(-1);
        }
        else if(hptr->type == SPACE)
            break;
        else if(CHECK_UNICODE_CHAR(hptr->val))
        {
            if(name_length != -1)
                free(name);
            PRINT_SYNTAX_ERROR(hptr);
            return(-1);
        }
        else
        {
            if(name_length == -1)
            {
                name =  malloc(SIZEOF_CHAR);
                name_length = 1;
            }
            else
            {
                name = realloc(name, (++name_length)*SIZEOF_CHAR);
            }
            name[name_length-1] = (char)hptr->val;
        }
        hptr = hptr->next;
    }
    name = realloc(name, (++name_length)*SIZEOF_CHAR);
    name[name_length-1] = '\0';

    printf("Name of for var: [%s]\n", name);

    free(name);
    return(0);
}


Token_Object *jumpOverIn(Token_Object *start, int set_null)
{
    Token_Object *hptr = start, *sav = NULL;

    //Jump over the spaces before the "in" keyword
    for(; hptr->type == SPACE; hptr=hptr->next);

    //Jump over the "in" keyword
    for(;hptr->type != SPACE; hptr=hptr->next);

    //Jump over the Spaces behind the "in" keyword
    for(;hptr->type == SPACE; sav = hptr, hptr=hptr->next);

    if(set_null)
        sav->next = NULL;

    return(hptr);
}


int checkForParms(LoopProperties *for_status)
{
    int max_length = 1000000000, i = 0;

    for(; i < for_status->length_of_vars; i++) 
    {
        if(for_status->vars[i].target.type == FLOAT ||
           for_status->vars[i].target.type == INTEGER ||
           for_status->vars[i].target.type == BOOL ||
           for_status->vars[i].target.type == STRING)
        {
            fprintf(stderr, "Format from [%s] is not supported in a for loop\n", for_status->vars[i].target.name);
            return(-1);
        }

        if(for_status->vars[i].target.array_length[0] < max_length)
            max_length = for_status->vars[i].target.array_length[0];
     }
    
     printf("For_length: [%d]\n", max_length);
     return(0);
}
