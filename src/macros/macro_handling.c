#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "token.h"
#include "variable.h"
#include "macros.h"

int macro_start(ParserStatus *status)
{
    status->in_status[MACRO_INDEX]++;
    if(status->mode == SAVE)
    {
        saveFromTree(status);
        return(0);
    }
    status->mode = SAVE;
    status->look_for = LOOK_FOR_MACRO;
    status->sav.head = status->token_tree.next;
    status->token_tree.next = NULL;
    status->found_block = 0;
    return(0);

}

int macro_end(ParserStatus *status)
{
    if(status->in_status[MACRO_INDEX] == 0)
    {
        fprintf(stderr, "End-for without for start\n");
        return(-1);
    }
    else if(--status->in_status[MACRO_INDEX] != 0)
    {
        saveFromTree(status);
        return(0);
    }
    else if(status->look_for != LOOK_FOR_MACRO)
    {
        saveFromTree(status);
        return(0);
    }

    Token_Object *macro_head = status->sav.head,
                 *start = NULL, *end = NULL;
    int i = 0;

    cleanTokenList(status->token_tree.next);

    status->sav.head = NULL;
    status->token_tree.next = NULL;
    status->mode = 0;
    status->cur_token = &status->token_tree;
    status->found_block = 0;

    //TODO: find a nicer way to jump to the macro name
    //example: {% macro test(test1, test2) %}
    //                 ^
    
    //Jump before the "macro" keyword
    for(start = macro_head;
        start != NULL && 
        start->type != SPACE
        ;start=start->next);

    //Jump over the "macro" keyword
    for(start = start->next;
        start != NULL && 
        start->type != SPACE
        ;start=start->next);

    //Jump over the spaces
    for(start = start->next;
        start != NULL && 
        start->type == SPACE
        ;start=start->next);

    end = start->next;
    i = 0;
    //Search end of command
    while(end->next)
    {
        if(end->next->type == CLOSEBRACKET)
            break;
        end = end->next;
    }
    cleanTokenList(end->next);
    end->next = NULL;


    start_save_macro(start, &status->sav);
}

int maco_exec(ParserStatus *status)
{
}


void printMacro(MacroDefinition *target)
{
    int i = 0;

    printf("Name: [%s]\n", target->name);
    for(; i < target->number_of_parms; i++)
    {
        printf("\t%s | required: %s\n", target->parms[i].name, 
            target->parms[i].required == 0 ? "no" : "yes");
    }
}

void freeMacro(MacroDefinition **target)
{
    int i = 0;

    for(; i < (*target)->number_of_parms; i++)
    {
        free((*target)->parms[i].name);
        free((*target)->parms[i].val);
    }
    free((*target)->parms);
    free((*target)->name);
    //TODO: Add free body

    free((*target));
}

void freeAllMacros(MacroEntries *target)
{
    int i = 0;

    for(; i < target->length; i++)
    {
        freeMacro(&target->macros[i]);
    }
    free(target->macros);
}
