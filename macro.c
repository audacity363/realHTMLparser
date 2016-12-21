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

#if 0
int save_arg()
#endif

int end_macro_handling(token_t *anker, status_t *stat)
{
    int i = 0, keyword_offset = 0;
    wchar_t macro_name[500],
            *macro_pos = NULL,
            **args_names = NULL;
    bool found_space = false;

    int args_number = 0;
    void **args_value = NULL;

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
            //memset(arg_val, 0x00, sizeof(arg_val));
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
            //memset(arg_name+name_index, 0x00, sizeof(wchar_t));
            memset(arg_val+val_index, 0x00, sizeof(wchar_t));
            printf("Arg_name: [%S]\n", arg_name);
            printf("\t[%S]\n", arg_val);
            memset(arg_val, 0x00, sizeof(arg_val));
            name_index = 0;
            val_index = 0;
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
    
   
    printf("----Macro body----\n");
    
    for(i=1; i < stat->sizeof_sav_buff; i++)
        printf("%S\n", stat->save_buff[i]);

    printf("------------------\n");

    return(0);
}
