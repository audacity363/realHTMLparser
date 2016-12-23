#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"
#include "command_parsing.h"
#include "macro.h"


int macro_handling(token_t *anker, status_t *stat)
{
    if(stat->just_save == true)
    {
        fprintf(stderr, "Macro definition not allowed in if or for block\n");
        return(EXIT);
    }

    stat->just_save = true;
    return(JUSTSAVE);
}

/*
 * Print all parameter from a macro 
 * Format: name
 *          value
 */
void printMacroParms(macro_parms *macro)
{
    int i = 0;

    for(; i < macro->parm_number; i++)
    {
        printf("name: [%s]\n", macro->name[i]);
        switch(macro->type[i])
        {
            case STRING:
                printf("\tval: [%S]\n", (wchar_t*)macro->val[i]);
                break;
            case INTEGER:
                printf("\tval: [%d]\n", *((int*)macro->val[i]));
                break;
            case BOOL:
                if(*((bool*)macro->val[i]) == true)
                    printf("\tval: [true]\n");
                else
                    printf("\tval: [false]\n");
                break;
            case FLOAT:
                printf("\tval: [%f]\n", *((double*)macro->val[i]));
                break;
            case -1:
                printf("\tno val\n");
                break;
        }
    }
}

void listAllMacros(macro_definition_t *macros)
{
    macro_definition_t *hptr;
    int i = 0;

    hptr = macros->next;

    while(hptr)
    {
        printf("macro_name: [%s]\n", hptr->name);
        printMacroParms(hptr->parms);
        hptr = hptr->next;
    }
}

/*
 * Saves a macro parm and parses the default value and saves it in the right format
 * Supported datatype: String, Integer, Float, Boolean
 * Should arrays be supported??
 *
 * TODO: Add Error handling for wc->mbc/wc->int/wc->double conversion
 */
int save_arg(macro_parms *macro, wchar_t *name, int found_val, wchar_t *val)
{
    int index = macro->parm_number, int_buff = 0;
    wchar_t *non_valid;
    double d_buff = 0;
    char *c_name = NULL;

    if((c_name = malloc(wcslen(name)+1)) == NULL)
    {
        fprintf(stderr, "[%s.%d]: malloc error\n", __FILE__, __LINE__);
        return(-1);
    }

    wcstombs(c_name, name, wcslen(name)+1);

    if((macro->name[index] = malloc(strlen(c_name))) == NULL)
    {
        fprintf(stderr, "[%s.%d]: malloc error\n", __FILE__, __LINE__);
        return(-2);
    }

    strcpy(macro->name[index], c_name);
    if(found_val == true)
    {
        //Check if string
        if(memcmp(val, L"\"", sizeof(wchar_t)) == 0)
        {
            macro->type[index] = STRING;
            //Copy value without quotation marks
            if((macro->val[index] = malloc((wcslen(val)-1)*sizeof(wchar_t))) == NULL)
            {
                fprintf(stderr, "[%s.%d]: malloc error\n",__FILE__, __LINE__);
                return(-1);
            }
            wcsncpy((wchar_t*)macro->val[index], val+1, wcslen(val)-2);
        }
        //Look for a float
        else if(wcschr(val, L'.') != NULL)
        {
            macro->type[index] = FLOAT;
            d_buff = wcstod(val, NULL);

            if((macro->val[index] = malloc(sizeof(double))) == NULL)
            {
                fprintf(stderr, "[%s.%d]: malloc error\n",__FILE__, __LINE__);
                return(-1);
            }

            *((double*)macro->val[index]) = d_buff;
        }
        //Look for true boolean
        else if(wcscmp(val, L"true") == 0)
        {
            macro->type[index] = BOOL;
            if((macro->val[index] = malloc(sizeof(bool))) == NULL)
            {
                fprintf(stderr, "[%s.%d]: malloc error\n",__FILE__, __LINE__);
                return(-1);
            }
            *((bool*)macro->val[index]) = true;

        }
        //Look for false boolean
        else if(wcscmp(val, L"false") == 0)
        {
            macro->type[index] = BOOL;
            if((macro->val[index] = malloc(sizeof(bool))) == NULL)
            {
                fprintf(stderr, "[%s.%d]: malloc error\n",__FILE__, __LINE__);
                return(-1);
            }
            *((bool*)macro->val[index]) = false;
        }
        //It is an integer
        else
        {
            macro->type[index] = INTEGER;
            int_buff = (int)wcstol(val, &non_valid, 10);

            if((macro->val[index] = malloc(sizeof(int))) == NULL)
            {
                fprintf(stderr, "[%s.%d]: malloc error\n",__FILE__, __LINE__);
                return(-1);
            }
            *((int*)macro->val[index]) = int_buff;
        }
    }
    else
    {
        macro->type[index] = -1;
        macro->val[index] = NULL;
    }

    macro->parm_number++;
    macro->name = realloc(macro->name, sizeof(char*)*(macro->parm_number+1));
    macro->val = realloc(macro->val, sizeof(void*)*(macro->parm_number+1));
    macro->type = realloc(macro->type, sizeof(int*)*(macro->parm_number+1));

    return(0);
}

int end_macro_handling(token_t *anker, status_t *stat)
{
    int i = 0, keyword_offset = 0;
    wchar_t macro_name[500],
            *macro_pos = NULL,
            **args_names = NULL;
    char *c_name;
    bool found_space = false;

    int args_number = 0;
    void **args_value = NULL;

    macro_parms *cur_macro = malloc(sizeof(macro_parms));

    token_t head = {' ', -1, NULL, NULL},
            *hptr = NULL;

    if(stat->in_if != 0)
    {
        fprintf(stderr, "Unclosed if block in macro\n");
        return(EXIT);
    }
    if(stat->in_for != 0)
    {
        fprintf(stderr, "Unclosed for block in macro\n");
        return(EXIT);
    }

    if((cur_macro->name = malloc(sizeof(char*))) == NULL)
    {
        fprintf(stderr, "[%s.%d]: malloc error\n",__FILE__, __LINE__);
        return(EXIT);
    }

    if((cur_macro->val = malloc(sizeof(void*))) == NULL)
    {
        fprintf(stderr, "[%s.%d]: malloc error\n",__FILE__, __LINE__);
        return(EXIT);
    }

    if((cur_macro->type = malloc(sizeof(int*))) == NULL)
    {
        fprintf(stderr, "[%s.%d]: malloc error\n",__FILE__, __LINE__);
        return(EXIT);
    }


    lineToTokens(&head, stat->save_buff[0], stat->save_buff[0]+wcslen(stat->save_buff[0]));
    
    hptr = head.next;

    if((macro_pos = wcsstr(stat->save_buff[0], L"macro")) == NULL)
    {
        fprintf(stderr, "Did not find macro keyword in macro head\n");
        return(EXIT);
    }

    keyword_offset = macro_pos - stat->save_buff[0];

    for(i=0; i < keyword_offset; i++)
    {
        hptr = hptr->next;
        if(!hptr)
        {
            fprintf(stderr, "Macro offset to high\n");
            return(EXIT);
        }
    }

    i=0;
    //Get Macro name
    while(hptr)
    {
        if(hptr->type == CLINGTO)
        {
            break;
        }
        else if(found_space == true)
        {
            memcpy(macro_name+(i++), &hptr->val, sizeof(wchar_t));
        }
        else if(hptr->type == SPACE)
        {
            found_space = true;
        }
        hptr = hptr->next;
    }

    memset(macro_name+i, 0x00, sizeof(wchar_t));

    printf("macro_name: [%S]\n", macro_name);


    //Get macro arguments with default values
    args_value = malloc(sizeof(void*));
    args_names = malloc(sizeof(wchar_t*));

    bool in_val = false;
    wchar_t arg_name[500], arg_val[500];
    int name_index = 0, val_index = 0, arg_type = 0;

    hptr = hptr->next;
    while(hptr)
    {
        //End of macro function
        if(hptr->type == CLAMPS)
        {
            //memset(arg_name+name_index, 0x00, sizeof(wchar_t));
            memset(arg_val+val_index, 0x00, sizeof(wchar_t));
            printf("Arg_name: [%S]\n", arg_name);
            printf("\t[%S]\n", arg_val);

            save_arg(cur_macro, arg_name, in_val, arg_val);

            name_index = 0;
            break;
        }
        //End of parm name
        else if(hptr->type == EQUALS) 
        {
            memset(arg_name+name_index, 0x00, sizeof(wchar_t));
            name_index = 0;
            val_index = 0;
            in_val = true;
            hptr = hptr->next;
            continue;
        }
        //End of parm name or end of parm value
        else if(hptr->type == COMMA)
        {
            memset(arg_val+val_index, 0x00, sizeof(wchar_t));

            printf("Arg_name: [%S]\n", arg_name);
            printf("\t[%S]\n", arg_val);

            save_arg(cur_macro, arg_name, in_val, arg_val);

            memset(arg_val, 0x00, sizeof(arg_val));
            name_index = val_index = 0;
            in_val = false;

            hptr = hptr->next;
            continue;
        }
        //Ignore Spaces between declarations
        else if(hptr->type == SPACE && in_val == false)
        {
            hptr = hptr->next;
            continue;
        }
        else if(in_val == false)
        {
            memcpy(arg_name+(name_index++), &hptr->val, sizeof(wchar_t));
        }
        else if(in_val == true)
        {
            memcpy(arg_val+(val_index++), &hptr->val, sizeof(wchar_t));
        }
        hptr = hptr->next;
    }
    
    printMacroParms(cur_macro);

    c_name = malloc(wcslen(macro_name)+1);
    wcstombs(c_name, macro_name, wcslen(macro_name)+1);

    saveMacro(c_name, cur_macro, stat);

   
    printf("----Macro body----\n");
    
    for(i=1; i < stat->sizeof_sav_buff; i++)
        printf("%S\n", stat->save_buff[i]);

    printf("------------------\n");

    return(0);
}

/*
 * Saves a new macro to the macro_defs stack.
 * TODO: Write a function thats frees all parm array, the body and the list
 */
int saveMacro(char *name, macro_parms *parms, status_t *body)
{
    macro_definition_t *hptr, *new;
    int i = 1;

    hptr = macro_defs;

    while(hptr->next != NULL)
        hptr->next;

    if((new = malloc(sizeof(macro_definition_t))) == NULL)
    {
        fprintf(stderr, "[%s.%d]: malloc error\n",__FILE__, __LINE__);
        return(-1);
    }

    new->name = malloc(strlen(name)+1);
    strcpy(new->name, name);
    new->body = malloc(sizeof(wchar_t*)*body->sizeof_sav_buff);
    for(; i < body->sizeof_sav_buff; i++)
        new->body[i-1] = body->save_buff[i];

    new->sizeof_body = body->sizeof_sav_buff;

    new->parms = parms;

    body->save_buff = NULL;
    body->sizeof_sav_buff = 0;

    hptr->next = new;

    return(0);
    
}

int initMacroAnker(macro_definition_t **anker)
{
    if((*anker = malloc(sizeof(macro_definition_t))) == NULL)
    {
        fprintf(stderr, "[%s.%d]: malloc error\n",__FILE__, __LINE__);
        return(-1);
    }

    (*anker)->name = NULL;
    (*anker)->body = NULL;
    (*anker)->sizeof_body = -1;
    (*anker)->parms = NULL;
    (*anker)->next = NULL;

    return(0);
}
