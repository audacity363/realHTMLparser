#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "token.h"
#include "parser_utils.h"
#include "variable.h"
#include "for.h"

int for_handling(ParserStatus *status) 
{
    status->in_status[FOR_INDEX]++;
    if(status->mode == SAVE)
    {
        saveFromTree(status);
        return(0);
    }
    status->mode = SAVE;
    status->look_for = LOOK_FOR_FOR;
    status->sav.head = status->token_tree.next;
    status->token_tree.next = NULL;
    status->found_block = 0;
    return(0);
}

int end_for_handling(ParserStatus *status) 
{
    if(status->in_status[FOR_INDEX] == 0)
    {
        fprintf(stderr, "End-for without for start\n");
        return(-1);
    }
    else if(--status->in_status[FOR_INDEX] != 0)
    {
        saveFromTree(status);
        return(0);
    }
    else if(status->look_for != LOOK_FOR_FOR)
    {
        saveFromTree(status);
        return(0);
    }


    Token_Object *for_head = status->sav.head, *start = NULL, *end = NULL;

    //Parse for head
    start = jumpOverCommand(for_head, 1);

    //Jump to the end of the block
    for(end=start; end->next->type != COMMANDBLOCK; end=end->next);
    cleanTokenList(end->next);
    end->next = NULL;

    return(exec_for(status, start));
}
