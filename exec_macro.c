#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"
#include "command_parsing.h"
#include "macro.h"

int getArguments(token_t *start, macro_parms *parms);

int exec_macro(token_t *anker, macro_definition_t *macro)
{
    int i = 0;

    macro_parms parms = {0, NULL, NULL, NULL};

    printf("exec Macro:\n");

    getArguments(anker, &parms);

    /*for(; i < macro->sizeof_body; i++)
        printf("%S\n", macro->body[i]);

    printf("---End Marco---\n");*/
}


int getArguments(token_t *start, macro_parms *parms)
{
    token_t *hptr = NULL;
    wchar_t name[500];
    int type = 0, index = 0;
    bool in_var = false;

    if(memcmp(&start->val, L"(", sizeof(wchar_t)) != 0)
    {
        fprintf(stderr, "Missing \"(\"\n");
        return(-1);
    }

    hptr = start->next;

    parms->type = malloc(sizeof(int));
    parms->val = malloc(sizeof(wchar_t*));

    while(hptr)
    {
        if(hptr->type == CLAMPS)
        {
            memset(name+(index++), 0x00, sizeof(wchar_t));
            printf("parm: [%S]\n", name);
            saveParm(name, parms);
            //save parm
            break;
        }
        else if(hptr->type == SPACE && in_var == true)
        {
            memcpy(name+(index++), &hptr->val, sizeof(wchar_t));
        }
        else if(hptr->type == STR)
        {
            in_var = !in_var;
            memcpy(name+(index++), &hptr->val, sizeof(wchar_t));
        }
        else if(hptr->type == CHAR)
        {
            memcpy(name+(index++), &hptr->val, sizeof(wchar_t));
        }
        else if(hptr->type == COMMA)
        {
            if(in_var == true)
            {
                fprintf(stderr, "missing '\"'\n");
                return(-2);
            }
            //save parm
            memset(name+(index++), 0x00, sizeof(wchar_t));
            printf("parm: [%S]\n", name);
            saveParm(name, parms);
            index=0;
        }
        
        hptr = hptr->next;
    }

}

/*
 * TODO: free the parms after using them
 * TODO: add group handling
 */
int saveParm(wchar_t *arg, macro_parms *parms)
{
    int index = parms->parm_number;
    double d_buff = 0;
    char *c_name = NULL;

    //Look for a String
    if(memcmp(arg, L"\"", sizeof(wchar_t)) == 0)
    {
        parms->type[index] = STRING;
        parms->val[index] = malloc(wcslen(arg)*sizeof(wchar_t));
        wcsncpy((wchar_t*)parms->val[index], arg+1, wcslen(arg)-1);
    }
    //Look for a float
    else if(wcschr(arg, L'.') != NULL)
    {
        parms->type[index] = FLOAT;

        d_buff = wcstod(arg, NULL);
        parms->val[index] = malloc(sizeof(double));
        *((double*)parms->val[index]) = d_buff;
    }
    //Look for a true boolean
    else if(wcscmp(arg, L"true") == 0)
    {
        parms->type[index] = BOOL;
        parms->val[index] = malloc(sizeof(bool));
        *((bool*)parms->val[index]) = true;
    }
    //Look for a false boolean
    else if(wcscmp(arg, L"false") == 0)
    {
        parms->type[index] = BOOL;
        parms->val[index] = malloc(sizeof(bool));
        *((bool*)parms->val[index]) = false;
    }
    //must be an integer or an variable
    else
    {
        c_name = malloc(wcslen(arg)+1);
        wcstombs(c_name, arg, wcslen(arg)+1);
        if(isDefinedBool(vars_anker, c_name) == false)
        {
            //Var ist not defined. It is an integer
            free(c_name);
            parms->type[index] = INTEGER;
        }
        else
        {
            parms->val[index] = malloc(strlen(c_name)+1);
            strcpy((char*)parms->val[index], c_name);
            //-1 means here that it is a variable
            parms->type[index] = -1;
            free(c_name);
        }
    }

    index = (++parms->parm_number);
    //just so i have to write less
    index++;
    parms->val = realloc(parms->val, index*sizeof(void*));
    parms->type = realloc(parms->type, index*sizeof(int));
}
