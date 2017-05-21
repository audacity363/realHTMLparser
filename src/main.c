#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "variable.h"
#include "dummy_functions.h"
#include "token.h"

void cleanBuffers(ParserStatus *status);

VariableObject *var_anker = NULL;
MacroEntries macros = {-1, NULL};
FILE *f_output = NULL;

int main(int argc, char *argv[])
{
    char *inputfile = "./tests/test.html",
         *outputfile = "./out.html";
    if(argc == 2)
    {
        inputfile = argv[1];
    }
    else if(argc == 3)
    {
        inputfile = argv[1];
        outputfile = argv[2];
    }

    //printf("Token obj: [%d]\n", sizeof(Token_Object));
    wchar_t cur_chr, *sav_buffer = NULL;

    printf("Using input: [%s]\n", inputfile);
    printf("Using output: [%s]\n", outputfile);

    FILE *fp;
    if((fp = fopen(inputfile, "r")) == NULL)
    {
        fprintf(stderr, "Can not open inputfile\n");
        return(-1);
    }

    if((f_output = fopen(outputfile, "w")) == NULL)
    {
        fprintf(stderr, "Can not open outputfile\n");
        return(-1);
    }

    int found_block = 0,
        col_no = 0,
        line_no = 0,
        save = 0,
        ret = 0;

    ParserStatus status = 
    {
        //Anker of token list
        { -1, L'\0', -1, -1, NULL },
        0,
        0,
        {0, 0, 0},
        { NULL, NULL, NULL, 0, 0, 0 },
        0,
        0,
        0,
        READ_FROM_FILE,
        NULL,
        NULL
    };

    status.cur_token = &status.token_tree;
    status.fp = fp;

    status.sav.length = malloc(sizeof(int));
    status.sav.length[0] = -1;
    status.sav.sav_buff = malloc(sizeof(wchar_t*));
    status.sav.sav_buff[0] = NULL;
    status.sav.cursor = malloc(sizeof(int));
    status.sav.cursor[0] = 0;

    initAnker(&var_anker);

    addNewGroup(var_anker, "newgrp");
    newString(var_anker, "newgrp", "test", 50);
    setString(var_anker, "newgrp", "test", L"Hello World");
    
    wchar_t tmp[21];
    int i,x;
    new2DString(var_anker, NULL, "test1", 20, 4, 6);

    for(i=0; i < 4; i++)
        for(x=0; x < 6; x++)
        {
            swprintf(tmp, 20, L"test%d%d", i, x);
            set2DStringXY(var_anker, NULL, "test1", i, x, tmp);
        }

    new3DString(var_anker, NULL, "test2", 20, 3, 4, 6);
    set3DStringXYZ(var_anker, NULL, "test2", 1, 0, 5, L"HEllo World");
    set3DStringXYZ(var_anker, NULL, "test2", 2, 3, 5, L"HEllo World111111111");

    newInteger(var_anker, NULL, "test3");
    setInteger(var_anker, NULL, "test3", 1);

    new1DInteger(var_anker, NULL, "test4", 5);
    set1DIntegerX(var_anker, NULL, "test4", 2, 1);

    addNewGroup(var_anker, "testgrp");
    new1DInteger(var_anker, "testgrp", "test1", 5);
    set1DIntegerX(var_anker, "testgrp", "test1", 1, 6);


    printAllVars(var_anker, stdout);

    while(1)
    {
        getCharfromFile(fp, &cur_chr);
        if(cur_chr == WEOF)
            break;

        if(cur_chr == L'\n')
        {
            status.cur_line++;
            if(status.cur_col == 0)
            {
                continue;
            }
            status.cur_col = 0;
        }
        else
            status.cur_col++;

        if((ret = parseChr(&status, cur_chr)) < 0 ||
            ret == BREAK)
        {
            break;
        }
    }

    fclose(fp);
    fclose(f_output);

    cleanBuffers(&status);
    freeVarAnker(var_anker);
    freeAllMacros(&macros);
}

void cleanBuffers(ParserStatus *status)
{
    fprintf(stderr, "Start free process\n");
    for(; status->sav.real_level > -1; status->sav.real_level--)
    {
        fprintf(stderr, "free level [%d]\n", status->sav.real_level);
        free(status->sav.sav_buff[status->sav.real_level]);
    }
    free(status->sav.sav_buff);
    free(status->sav.cursor);
    free(status->sav.length);

    cleanTokenList(status->token_tree.next);
    cleanTokenList(status->sav.head);
}

int saveChr(SaveObject *sav, wchar_t cur_chr)
{
    int level = 0;
    if(sav->real_level > sav->level)
    {
        for(; sav->real_level != sav->level; sav->real_level--)
        {
            D(printf("Free level [%d]\n", sav->real_level));
            free(sav->sav_buff[sav->real_level]);
        }
        sav->sav_buff = realloc(sav->sav_buff, (sav->real_level+2)*sizeof(wchar_t*));
        sav->length = realloc(sav->length, (sav->real_level+2)*sizeof(int));

        //Clean the buffer on the current level
        free(sav->sav_buff[sav->real_level]);
        sav->sav_buff[sav->real_level] = NULL;
        sav->length[sav->real_level] = -1;
    }
    else if(sav->real_level < sav->level)
    {
        //Between the last save and now there was a commandblock
        sav->sav_buff = realloc(sav->sav_buff, (sav->real_level+2)*sizeof(wchar_t*));
        sav->length = realloc(sav->length, (sav->real_level+2)*sizeof(int));
        sav->real_level++;
        sav->sav_buff[sav->real_level] = NULL;
        sav->length[sav->real_level] = -1;
    }

    //The Buffer is empty
    if(sav->length[sav->real_level] == -1)    
    {
        sav->sav_buff[sav->real_level] = malloc(SIZEOF_WCHAR);
        sav->length[sav->real_level] = 1;
    }
    //Append the buffer
    else
    {
        ++sav->length[sav->real_level];
        sav->sav_buff[sav->real_level] = realloc(sav->sav_buff[sav->real_level], 
            SIZEOF_WCHAR*(sav->length[sav->real_level]));
    }
    sav->sav_buff[sav->real_level][(sav->length[sav->real_level])-1] = cur_chr;
    return(0);
}

int saveFromTree(ParserStatus *stat)
{
    SaveObject *sav = &stat->sav;
    Token_Object *start = stat->token_tree.next;
    int found_end_command = 0;

    while(start)
    {
        if(start->next && !found_end_command)
        {
            if(start->type == COMMANDBLOCK && 
                start->next->type == BLOCKEND)
            {
                found_end_command = 1;
            }
        }
        saveChr(sav, start->val);
        start = start->next;
    }
    if(!found_end_command)
        saveChr(sav, L'}');
}

int getCharfromFile(FILE *fp, wchar_t *chr)
{
    *chr = getwc(fp);
}
