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
        *cursor = NULL,
        start_line = 0, save_line = 0, save_col = 0,
        i = 0;

    wchar_t *for_body = status->sav.sav_buff[for_level];

    LoopProperties for_status = {-1, -1, -1, NULL};

    //cleanTokenList(status->token_tree.next);

    start_line = start->line_no+1;
    save_line = status->cur_line; save_col = status->cur_col;


    if(parseForHead(start, &for_status) < 0)
    {
        return(-1);
    }

    for_status.length = getForLength(&for_status);
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
    
    printf("Length of the loop: [%d]\n", for_status.length);

    initForVariables(var_anker, &for_status);

    MY_FOR_HEAD(for_status)
    {
        nextForVariable(&for_status);

        status->cur_line = start_line;
        status->cur_col = 0;

        for(status->sav.cursor[for_level]=0;
            status->sav.cursor[for_level] < for_length;
            status->sav.cursor[for_level]++)
        {
            if(for_body[status->sav.cursor[for_level]] == L'\n')
            {
                status->cur_line++;
                if(status->cur_col == 0)
                {
                    continue;
                }
                status->cur_col = 0;
            }
            else
            {
                status->cur_col++;
            }

            D(printf("Sending: [%C] on [%d/%d]\n", 
                (iswprint(for_body[status->sav.cursor[for_level]]) != 0 
                ? for_body[status->sav.cursor[for_level]] : L'.'),
                status->sav.cursor[for_level], for_length));

            parseChr(status, for_body[status->sav.cursor[for_level]]);
        }
    }
    status->sav.level--;
    cleanupForVariables(&for_status);

    printf("Save line: [%d]\n", save_line);
    status->cur_line = save_line;
    status->cur_col  = save_col;

    status->read_from = READ_FROM_FILE;
    return(0);
}
