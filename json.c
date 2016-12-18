#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"

int exec_json(token_t *anker, status_t *stat)
{
    char **varnames = NULL;
    wchar_t name_buff[200];
    memset(name_buff, 0x00, sizeof(name_buff));

    int index_name = 0,
        var_count = 0;
    
    token_t *hptr = NULL;

    varnames = malloc(sizeof(char*));

    hptr = anker;

    while(hptr)
    {
        if(hptr->type == COMMA || hptr->type == CLAMPS)
        {
            memset(name_buff+index_name, 0x00, sizeof(wchar_t));
            varnames[var_count] = malloc(sizeof(char)*wcslen(name_buff));

            wcstombs(varnames[var_count], name_buff, wcslen(name_buff)+1);

            varnames = realloc(varnames, sizeof(char*)*((++var_count)+1));

            index_name = 0;
        }
        else if(hptr->type == CHAR)
        {
            memcpy(name_buff+index_name, &hptr->val, sizeof(wchar_t));
            index_name++;
        }
        hptr = hptr->next;
    }

    //printAllVarsJSON(vars_anker);
    if(printVarsToFileJSON(vars_anker, varnames, var_count, stdout) != 0)
        printf("Kenn ich nicht\n");
    return(0);
}
