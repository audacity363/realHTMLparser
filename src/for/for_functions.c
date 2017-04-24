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

int range_function(char *name, VariableObject *target);

int checkIfFunction(char *name, LoopProperties *for_status)
{
    char *function_names[] = {"range"};
    int i = 0;

    int (*functions[])(char*, VariableObject*) = {range_function};

    for(; i < sizeof(function_names)/sizeof(char*); i++)
    {
        if(strncmp(function_names[i], name, strlen(function_names[i])) == 0)
        {
            functions[i](name, &for_status->vars[for_status->length_of_vars-1].target);
        }
    }
    return(-1);
}

int range_function(char *name, VariableObject *target)
{
    printf("function: [%s]\n", name);
    return(0);
}
