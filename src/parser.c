#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "dummy_functions.h"
#include "token.h"
#include "variable.h"
#include "macros.h"

char *commands[] = 
{
    "for",
    "end-for",
    "if", 
    "end-if",
    "genJSON",
    "printVars",
    "dumpVars",
    "typeof",
    "macro",
    "end-macro",
};

int (*function_dic[])(ParserStatus *) = {
    for_handling,
    end_for_handling,
    if_handling,
    end_if_handling,
    gen_json,
    print_vars,
    dump_vars,
    typeof_func,
    macro_start,
    macro_end
};



int parseChr(ParserStatus *status, wchar_t p_chr)
{
    int ret = 0;
    wchar_t chr = p_chr;

    if(chr == L'{' && !status->found_block)
    {
        status->found_block = 1;
        status->cur_token = safeToken(status->cur_token, chr, 
            status->cur_line, status->cur_col);
        return(0);
    }

    if(status->mode != SAVE && !status->found_block)
        printf("%C", chr);

    if(status->mode == SAVE && !status->found_block)
        saveChr(&status->sav, chr);

    if(chr == L'}')
    {
        //End of command block
        if(status->cur_token->val == L'%')
        {
            status->cur_token = safeToken(status->cur_token, chr,
                status->cur_line, status->cur_col);

            if((ret = checkBlock(status)) < 0)
            {
                cleanTokenList(status->token_tree.next);
                status->token_tree.next = NULL;
                return(-1);
            }

            cleanTokenList(status->token_tree.next);
            status->token_tree.next = NULL;

            status->cur_token = &status->token_tree;
            status->found_block = 0;
            return(0);
        }

        //Check if it is the end of an variablen block
        if(status->read_from == READ_FROM_FILE)
            getCharfromFile(status->fp, &chr);
        else {
            (status->sav.cursor[status->sav.level-1])++;
            getCharfromBuffer(&status->sav, &chr);
            //printf("Read next char from buffer[%C]\n", chr);
        }

        if(chr == WEOF)
            return(BREAK);

        if(chr == L'}')
        {
            status->cur_token = safeToken(status->cur_token, chr, 
                status->cur_line, status->cur_col);
            //printTokens(&TokenList);

            checkBlock(status);

            cleanTokenList(status->token_tree.next);
            status->token_tree.next = NULL;

            status->cur_token = &status->token_tree;
            status->found_block = 0;
        }
        else
            status->cur_token = safeToken(status->cur_token, chr, 
                status->cur_line, status->cur_col);
    }
    else if(status->found_block)
        status->cur_token = safeToken(status->cur_token, chr, 
            status->cur_line, status->cur_col);

    return(0);
}

int checkBlock(ParserStatus *stat)
{
    Token_Object *tree = stat->token_tree.next,
                 *sav = tree;
    Token_Object *cmd_start = NULL;
    char *cmd_name = NULL;

    int i = 0,
        (*exec_func)(ParserStatus *) = NULL;

    //Check if it is a variable block
    if(tree->type == VARIABLEBLOCK && tree->next->type == VARIABLEBLOCK)
    {
        //save the block when we are in a if, for or macro block
        if(stat->mode == SAVE)
        {
            //printf("Dont exec just save\n");
            saveFromTree(stat);
        }
        else
        {
            //printfromTree(stat->token_tree.next);
            //printf("[here goes the content of the variable]");
            handle_variable(stat);
        }
        stat->found_block = 0;
        return(0);
    }

    
    if(tree->type == VARIABLEBLOCK && tree->next->type != COMMANDBLOCK)
    {
        PRINT_SYNTAX_ERROR(tree->next);
        return(-1);
    }

    cmd_start = tree->next->next;
    if((cmd_name = getFirstCommand(&cmd_start)) == NULL)
    {
        return(-1);
    }
    D(printf("Found command: [%s]\n", cmd_name));
    for(; i < sizeof(commands)/sizeof(char*); i++)
    {
        if(strcmp(commands[i], cmd_name) == 0)
        {
            exec_func = function_dic[i];
            break;
        }
    }
    //printf("Found function: [0x%p]\n", exec_func);
    if(exec_func == NULL)
    {
        D(fprintf(stderr, "Unkown command\n"));
        return(EXIT);
    }

    exec_func(stat);

    free(cmd_name);
    return(0);
}

char *getFirstCommand(Token_Object **tree)
{
    int i = 0;
    Token_Object *hptr = *tree;

    char *command = malloc(SIZEOF_CHAR);

    //Ignore leading spaces
    for(;hptr->type == SPACE && hptr->type != COMMANDBLOCK; hptr = hptr->next);

    //read the name of the cmd
    for(;hptr->type != SPACE && hptr->type != COMMANDBLOCK &&
         hptr->val != '(' ;i++, hptr = hptr->next)
    {
        if(CHECK_UNICODE_CHAR(hptr->val))
        {
            PRINT_SYNTAX_ERROR(hptr);
            free(command);
            return(NULL);
        }
        command[i] = (char)hptr->val;
        command = realloc(command, SIZEOF_CHAR*(i+2));
    }
    command[i] = '\0';

    *tree = hptr;
    return(command);
}
