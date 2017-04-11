#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "token.h"
#include "variable.h"
#include "macros.h"

Token_Object *getMacroName(Token_Object *start, char **name, int *length);
int parseMacroParameter(Token_Object *start, MacroDefinition *macro);
int interpretationMacroParameter(Token_Object *entry, MacroDefinition *macro);

int start_save_macro(ParserStatus *status, Token_Object *start, SaveObject *sav_buff)
{
    int name_length = 0, i = 0, found_not_required = 0;
    MacroDefinition *macro = NULL;

    Token_Object *parm_start = NULL;

    macro = malloc(sizeof(MacroDefinition));
    memset(macro, 0x00, sizeof(MacroDefinition));
    macro->number_of_parms = -1;
    //macro->body_length = -1;

    //printTokens(start);
    parm_start = getMacroName(start, &macro->name, &name_length);

    parseMacroParameter(parm_start, macro);

    //Check if required parms are on the end of the declaration

    printMacro(macro);
    for(; i < macro->number_of_parms; i++)
    {
        if(macro->parms[i].required == 0)
            found_not_required = 1;
        if(macro->parms[i].required == 1 && found_not_required == 1)
        {
            //TODO: Free macro
            fprintf(stderr, "SyntaxError on Macro declaration parameter %i\n", i+1);
            return(-1);
        }
    }
    macro->body = status->sav.sav_buff[status->sav.real_level];
    macro->body_length = status->sav.length[status->sav.real_level];

    status->sav.sav_buff = NULL;
    status->sav.length = NULL;
    if(macros.length == -1)
    {
        macros.macros = malloc(sizeof(MacroDefinition*));
        memset(macros.macros, 0x00, sizeof(MacroDefinition*));
        macros.length = 1;
    }
    else
        macros.macros = realloc(macros.macros, (++macros.length)*sizeof(MacroDefinition*));

    macros.macros[macros.length-1] = macro;

    return(0);
}


//gets the name of the macro. Return the Token after the name and writes the
//name into name and the length int length
Token_Object *getMacroName(Token_Object *start, char **name, int *length)
{
    Token_Object *hptr = start;
    int _length = -1;

    for(;hptr->next != NULL && hptr->type != OPENBRACKET; hptr=hptr->next)
    {
        if(hptr->type == SPACE)
            continue;
        else if(CHECK_UNICODE_CHAR(hptr->val))
        {
            if(_length != -1)
                free(*name);
            PRINT_SYNTAX_ERROR(hptr);
            return(NULL);
        }
        if(_length == -1)
        {
            *name = malloc(SIZEOF_CHAR);
            _length = 1;
        }
        else
        {
            *name = realloc(*name, (++_length)*SIZEOF_CHAR);
        }
        (*name)[_length-1] = (char)hptr->val;
    }

    
    *name = realloc(*name, (++_length)*SIZEOF_CHAR);
    (*name)[_length-1] = '\0';
    *length = _length;
    return(hptr);
}

int parseMacroParameter(Token_Object *start, MacroDefinition *macro)
{
    Token_Object *hptr = NULL, **entries = NULL, *savptr = NULL; 

    int length_of_entries = -1, i = 0; 

    //Split complete aguments into an array
    hptr = start->next; 
    entries = malloc(sizeof(Token_Object*)); length_of_entries = 1;
    entries[0] = start->next;

    //No args was defined
    if(!hptr)
        return(0);

    while(hptr->next)
    {
        if(hptr->next->type == COMMA)
        {
            savptr = hptr->next->next;
            entries = realloc(entries, sizeof(Token_Object*)*(++length_of_entries));
            entries[length_of_entries-1] = savptr;
            
            hptr->next = NULL;
            hptr = savptr;
            continue;
        }
        hptr = hptr->next;
    }
    
    for(i=0; i < length_of_entries; i++)
    {
        interpretationMacroParameter(entries[i], macro);
        //printTokens(entries[i]);
        //printfromTree(entries[i]);
        //printf("\n");
    }

    free(entries);
    return(0);
}

int interpretationMacroParameter(Token_Object *entry, MacroDefinition *macro)
{
    char *buffer = NULL;     
    int length = -1;

    Token_Object *hptr = entry;
    MacroParms *target = NULL;

    if(macro->number_of_parms == -1)
    {
        macro->number_of_parms = 1;
        macro->parms = malloc(sizeof(MacroParms));
        memset(&macro->parms[0], 0x00, sizeof(MacroParms));
    }
    else
    {
        macro->parms = realloc(macro->parms, (++macro->number_of_parms)*sizeof(MacroParms));
        memset(&macro->parms[macro->number_of_parms-1], 0x00, sizeof(MacroParms));
    }

    target = &macro->parms[macro->number_of_parms-1];
    
    while(hptr)
    {
        if(hptr->val == L'=')
        {
            hptr = hptr->next;
            break;
        }
        else if(hptr->type == SPACE)
        {
            hptr = hptr->next;
            continue;
        }
        else if(CHECK_UNICODE_CHAR(hptr->val))
        {
            if(length != -1)
                free(buffer);
            PRINT_SYNTAX_ERROR(hptr);
            return(-1);
        }
        if(length == -1)
        {
            buffer = malloc(SIZEOF_CHAR);
            length = 1;
        }
        else
            buffer = realloc(buffer, (++length)*SIZEOF_CHAR);

        buffer[length-1] = (char)hptr->val;
        hptr = hptr->next;
    }

    buffer = realloc(buffer, (++length)*SIZEOF_CHAR);
    buffer[length-1] = '\0';

    target->name = malloc(length*SIZEOF_CHAR);
    strcpy(target->name, buffer);
    target->required = 1;

    if(hptr)
    {
        target->required = 0;
        parseStaticType(hptr, &target->type, &target->val);
    }

    free(buffer);

    return(0);
}
