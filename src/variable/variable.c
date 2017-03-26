#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "variable.h"

int handle_variable(ParserStatus *status)
{
    VariableParseData var_data = 
    {
        -1, NULL, -1, -1, NULL
    };

    printf("\n----Variable----\n");
    //printTokens(status->token_tree.next);
    getVariableAttributes(status->token_tree.next, &var_data);

    //execAttributes(&var_data);
    printf("----------------\n");

    freeVariableData(&var_data);

    return(0);
}
