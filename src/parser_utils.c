#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>

#include "parser.h"
#include "parser_utils.h"

int isdigit_str(char *str)
{
    int i = 0;
    for(; i < strlen(str); i++)
    {
        if(!isdigit(str[i]))
            return(0);
    }
    return(1);
}

//Jumps over the command name and return a pointer to the first char.
//when set_null is > 0 the space after teh command name whille be set
//to null
Token_Object *jumpOverCommand(Token_Object *start, int set_null)
{
    //example: {% macro test(test1, test2) %}
    //         ^
   
    Token_Object *hptr = NULL, *sav = NULL;
    
    //Check if the command is empty
    if(start->next->next == NULL)
        return(NULL);

    hptr = start->next->next;
    //Check if we have to jump over spaces between the command block and the cmd name
    if(hptr->type == SPACE)
    {
        for(; hptr->type == SPACE; hptr=hptr->next);
    }

    //example: {% macro test(test1, test2) %}
    //            ^

    //Jump over the command name
    for(;hptr->type != SPACE; hptr=hptr->next);
    //example: {% macro test(test1, test2) %}
    //                 ^
    
    //Jump over the remaining spaces
    for(;hptr->type == SPACE; sav=hptr, hptr=hptr->next);
    //example: {% macro test(test1, test2) %}
    //                  ^

    if(set_null)
        sav->next = NULL;
    return(hptr);
}

