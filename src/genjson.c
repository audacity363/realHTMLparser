#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "dummy_functions.h"
#include "token.h"
#include "variable.h"

int gen_json(ParserStatus *status)
{
    if(status->mode == SAVE)
    {
        saveFromTree(status);
        return(0);
    }

    Token_Object *start = NULL, *end = NULL, *hptr = NULL, *savptr = NULL,
                 **entries = NULL;

    VariableParseData var_data = 
    {
        {NULL, -1, NULL, 0, {0, 0, 0}, 0, 0, NULL, NULL, NULL}, -1, NULL
    };

    int i = 1, length_of_entries = -1;

    //Jump over the command
    for(start = status->token_tree.next;
        start != NULL && 
        start->type != OPENBRACKET
        ;start=start->next);

    end = start->next;

    //Search end of command
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

    //Split complete aguments into an array
    hptr = start->next; 
    entries = malloc(sizeof(Token_Object*)); length_of_entries = 1;
    entries[0] = start->next;

    while(hptr->next)
    {
        if(hptr->next->type == COMMA)
        {
            savptr = hptr->next->next;
            entries = realloc(entries, sizeof(Token_Object*)*(++length_of_entries));
            entries[length_of_entries-1] = savptr;
            
            hptr->next = NULL;
            hptr = savptr;
        }
        hptr = hptr->next;
    }
   
    fprintf(f_output, "{");
    for(i=0; i < length_of_entries; i++)
    {
        getVariableAttributes(entries[i], &var_data);

        //printAttributes(&var_data);
        
        execAttributes(&var_data);
        printVarPtr(&var_data.target, PRINT_MODE_JSON, f_output);
        if(i+1 < length_of_entries)
            fprintf(f_output, ",");
        freeVariableData(&var_data);
        //cleanTokenList(entries[i]);
    }
    fprintf(f_output, "}");
    free(entries);

    return(0);
}
