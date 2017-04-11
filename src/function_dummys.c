#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "parser.h"
#include "dummy_functions.h"
#include "token.h"

int if_handling(ParserStatus *status) {}
int end_if_handling(ParserStatus *status) {}

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

void getCharfromBuffer(SaveObject *sav, wchar_t *chr, int type)
{
#if 0
    printf("From Buffer: lvl: [%d] cursor [%d]\n", sav->level, sav->cursor[sav->level-1]);
    printf("Read from buffer [%S]\n", sav->sav_buff[sav->real_level-1]);
#endif

    switch(type)
    {
        case READ_FROM_BUFFER:
            (sav->cursor[sav->level-1])++;
            *chr = sav->sav_buff[sav->level-1][sav->cursor[sav->level-1]];
            break;
        case READ_FROM_MACRO_BUFFER:
            sav->macro.cursor++;
            *chr = sav->macro.body[sav->macro.cursor];
            break;
    }
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

    int for_level = status->sav.real_level,
        for_length = status->sav.length[for_level],
        *cursor = NULL;

    wchar_t *for_body = status->sav.sav_buff[for_level];
    Token_Object *for_head = status->sav.head;

    //for_body = realloc(for_body, (for_length+1)*SIZEOF_WCHAR);

    cleanTokenList(status->token_tree.next);

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
    cleanTokenList(for_head);
}
