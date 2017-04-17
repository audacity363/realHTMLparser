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

int exec_for(ParserStatus *status, Token_Object *start)
{
    int for_level = status->sav.real_level,
        for_length = status->sav.length[for_level],
        *cursor = NULL;

    wchar_t *for_body = status->sav.sav_buff[for_level];

    LoopProperties for_status = {-1, -1, -1, NULL};

    cleanTokenList(status->token_tree.next);

    if(parseForHead(start, &for_status) < 0)
    {
        return(-1);
    }

    if(checkForParms(&for_status))
    {
        return(-1);
    }
    //Start running the for loop

    status->sav.head = NULL;
    status->token_tree.next = NULL;
    status->mode = 0;
    status->cur_token = &status->token_tree;
    status->found_block = 0;
    D(fprintf(stderr, "Level: [%d]\n", for_level));
    
    status->sav.cursor = realloc(status->sav.cursor, (for_level+2)*sizeof(int));
    status->sav.cursor[for_level+1] = 0;

    if(status->sav.cursor == NULL)
    {
        fprintf(stderr, "realloc error\n");
        abort();
    }
    cursor = &status->sav.cursor[for_level];

    D(printf("Length of for body: [%d]\n", for_length));
    D(printf("For body: [%*.S]\n", for_length, for_body));
    D(printf("Executing....\n"));

    status->read_from = READ_FROM_BUFFER;
    status->look_for = -1;
    status->in_status[FOR_INDEX] = 0;
    status->sav.level++;

    //Can't use a pointer to the cursor array index because this function 
    //gets called recursive. So the realloc for the cursor array could move the
    //memory to a nother location and the pointer would be invalid

    for(status->sav.cursor[for_level]=0;
        status->sav.cursor[for_level] < for_length;
        status->sav.cursor[for_level]++)
    {
        D(printf("Sending: [%C] on [%d/%d]\n", 
            (iswprint(for_body[status->sav.cursor[for_level]]) != 0 
            ? for_body[status->sav.cursor[for_level]] : L'.'),
            status->sav.cursor[for_level], for_length));

        parseChr(status, for_body[status->sav.cursor[for_level]]);
    }
    status->sav.level--;

    return(0);
}
