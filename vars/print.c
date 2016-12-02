#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "print.h"

void printAllVarsToFile_in(vars_t *anker, FILE *fp, int offset);
void printInteger(vars_t *var, FILE *fp);
void print1DInteger(vars_t *var, FILE *fp);
void print2DInteger(vars_t *var, FILE *fp);
void printGroup(vars_t *var, FILE *fp);

#define SIZEOF_PRINT_FUNCS 4

int print_function_dic_i[SIZEOF_PRINT_FUNCS] = {
        INTEGER,
        GROUP,
        ONEDINTEGER,
        TWODINTEGER
    };

void (*print_function_dic_v[SIZEOF_PRINT_FUNCS])(vars_t*, FILE*) = {
        printInteger,
        printGroup,
        print1DInteger,
        print2DInteger
    };

void printAllVars(vars_t *anker)
{
    printAllVarsToFile(anker, stdout);
}

void printAllVarsToFile(vars_t *anker, FILE *fp)
{
    printAllVarsToFile_in(anker, fp, 0);
}


void printAllVarsToFile_in(vars_t *anker, FILE *fp, int offset)
{
    int i, x;
    vars_t *hptr;

    if(anker->name == NULL)
        hptr = anker->next;
    else
        hptr = anker;

    void (*print_func)(vars_t*, FILE*);

    while(hptr)
    {
        for(i=0; i < SIZEOF_PRINT_FUNCS; i++)
        {
            if(print_function_dic_i[i] == hptr->type)
            {
                for(x=0; x < offset; x++)
                    printf("\t");
                print_func = print_function_dic_v[i];
                print_func(hptr, fp);
            }
        }
        hptr = hptr->next;
    }
}


void printInteger(vars_t *var, FILE *fp)
{
    fprintf(fp, "[%s] = [%d]\n", var->name, *((int*)var->data));
}

void print1DInteger(vars_t *var, FILE *fp)
{
    int i;

    fprintf(fp, "[%s] = [", var->name);
    for(i=0; i < var->x_length; i++)
    {
        fprintf(fp, "%d", ((int*)var->data)[i]);
        if(i+1 < var->x_length)
        {
            fprintf(fp, ", ");
        }
    }
    fprintf(fp, "]\n");
}

void print2DInteger(vars_t *var, FILE *fp)
{
    int x, y;

    fprintf(fp, "[%s] = [", var->name);
    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "%d", ((int*)var->data)[var->x_length*x+y]);
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");

        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]\n");
}


void printGroup(vars_t *var, FILE *fp)
{
    printf("Group: [%s]:\n", var->name);
    printAllVarsToFile_in(var->next_lvl, fp, 1);
}
