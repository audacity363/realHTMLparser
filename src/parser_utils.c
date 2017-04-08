#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

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

