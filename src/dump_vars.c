#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "token.h"

//Dumps alls vars in a formated layout
int print_vars(ParserStatus *status)
{
    if(status->mode == SAVE)
    {
        saveFromTree(status);
        return(0);
    }
    fprintf(stdout, "<pre>");
    printAllVars(var_anker, stdout);
    fprintf(stdout, "</pre>");
    return(0);
}

//Dumps alls vars in json format
int dump_vars(ParserStatus *status)
{
    if(status->mode == SAVE)
    {
        saveFromTree(status);
        return(0);
    }
    fprintf(stdout, "{");
    printAllVarsJson(var_anker, stdout);
    fprintf(stdout, "}");
    return(0);
}
