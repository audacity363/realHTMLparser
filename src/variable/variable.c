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
        {NULL, -1, NULL, 0, {0, 0, 0}, 0, 0, NULL, NULL, NULL}, -1, NULL
    };

    //printTokens(status->token_tree.next);
    getVariableAttributes(status->token_tree.next, &var_data);

    execAttributes(&var_data);

    printVarPtr(&var_data.target, PRINT_MODE_RAW, stdout);

    freeVariableData(&var_data);

    return(0);
}
