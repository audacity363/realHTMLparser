#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "token.h"
#include "variable.h"
#include "macros.h"

int getNumberOfRequiredArgs(MacroDefinition *def);
int createVarList(VariableObject *anker, VariableParseData *entries, int length);
int interpretMacroBody(VariableObject *anker, ParserStatus *status, MacroDefinition *def);

MacroDefinition *searchMacro(MacroEntries *anker, char *name)
{
    int i = 0;

    for(; i < anker->length; i++)
    {
        if(strcmp(anker->macros[i]->name, name) == 0)
            return(anker->macros[i]);
    }
    return(NULL);
}


int execMacro(ParserStatus *status, MacroDefinition *def)
{
    printf("Exec Macro: [%s]\n", def->name);
    
    if(status->mode == SAVE)
    {
        saveFromTree(status);
        return(0);
    }

    Token_Object *start = NULL, *end = NULL, *hptr = NULL, *savptr = NULL,
                 **entries = NULL;

    VariableParseData *var_data = NULL;

    int i = 1, length_of_entries = -1, length_of_var_data = -1;

    VariableObject *macro_var_anker = NULL;

    //Jump over the command
    for(start = status->token_tree.next;
        start != NULL && 
        start->type != OPENBRACKET
        ;start=start->next);

    status->token_tree.next = NULL;
    status->mode = 0;
    status->cur_token = &status->token_tree;
    status->found_block = 0;

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

    if(start->next->type != CLOSEBRACKET)
    {
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
    }
    else
    {
        printf("No agrumets given\n");
        length_of_entries = 0;
    }
    
    int tmp = getNumberOfRequiredArgs(def);
    printf("parms:  [%d]/[%d]\n", length_of_entries, tmp);
    if( tmp > length_of_entries)
    {
        fprintf(stderr, "Missing args\n");
        free(entries);
        return(-1);
    }

    //Parse and interpret the arguments
    for(i=0; i < length_of_entries; i++)
    {
        if(length_of_var_data == -1)     
        {
            var_data = malloc(sizeof(VariableParseData));
            memset(&var_data[0], 0x00, sizeof(VariableParseData));
            var_data[0].number_of_attributes = -1;
            length_of_var_data = 1;
        }
        else
        {
            var_data = realloc(var_data, (++length_of_var_data)*sizeof(VariableParseData));
            memset(&var_data[length_of_var_data-1], 0x00, sizeof(VariableParseData));
            var_data[length_of_var_data-1].number_of_attributes = -1;
        }
        
        if(getVariableAttributes(entries[i], &var_data[length_of_var_data-1]) < 0)
        {
            fprintf(stderr, "Error in getVariableAttributes()\n");
            free(entries);
            return(-1);
        }
        printAttributes(&var_data[length_of_var_data-1]);
        if(execAttributes(&var_data[length_of_var_data-1]) < 0)
        {
            free(entries);
            return(-1);
        }
        free(var_data[length_of_var_data-1].target.name);
        var_data[length_of_var_data-1].target.name = malloc((strlen(def->parms[i].name)+1)*SIZEOF_CHAR);
        strcpy(var_data[length_of_var_data-1].target.name, def->parms[i].name);
    }

    initAnker(&macro_var_anker);

    createVarList(macro_var_anker, var_data, length_of_var_data);

    interpretMacroBody(macro_var_anker, status, def);

    free(entries);

    return(0);
}

int getNumberOfRequiredArgs(MacroDefinition *def)
{
    int i = 0;

    for(; i < def->number_of_parms; i++)
    {
        if(def->parms[i].required == 0)
            break;
    }
    return(i);
}

//TODO: create new Vars based on the not set optional parms
//TODO: create internal variable: __name__ = name of the called macro
int createVarList(VariableObject *anker, VariableParseData *entries, int length)
{
    VariableObject *hptr = anker;
    int i = 0;

    for(; hptr->next != NULL; hptr=hptr->next);

    for(; i < length; i++)
    {
        hptr->next = &entries[i].target;
        entries[i].target.prev = hptr;
        hptr = hptr->next;
    }

    printAllVars(anker, stdout);
    return(0);
}

int interpretMacroBody(VariableObject *anker, ParserStatus *status, MacroDefinition *def)
{
    int i = 0;

    VariableObject *sav = var_anker;

    var_anker = anker;

    status->read_from = READ_FROM_MACRO_BUFFER;
    status->look_for = -1;

    status->sav.macro.body = def->body;
    status->sav.macro.length_of_body = def->body_length;

    status->cur_line = 0;
    status->cur_col = 0;

    for(status->sav.macro.cursor = 0; 
        status->sav.macro.cursor < def->body_length;
        status->sav.macro.cursor++)
    {
        if(def->body[status->sav.macro.cursor] == L'\n')
        {
            status->cur_line++;
            status->cur_col = 0;
            continue;
        }
        status->cur_line = 0;
        status->cur_col = 0;
        parseChr(status, def->body[status->sav.macro.cursor]);
    }

    var_anker = sav;
    return(0);
}

