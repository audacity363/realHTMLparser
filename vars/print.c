#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "print.h"

void printAllVarsToFile_in(vars_t *anker, FILE *fp, int offset);
void printInteger(vars_t *var, FILE *fp);
void print1DInteger(vars_t *var, FILE *fp);
void print2DInteger(vars_t *var, FILE *fp);
void print3DInteger(vars_t *var, FILE *fp);
void printsingleBoolean(bool val, FILE *fp);
void printBoolean(vars_t *var, FILE *fp);
void print1DBoolean(vars_t *var, FILE *fp);
void print2DBoolean(vars_t *var, FILE *fp);
void print3DBoolean(vars_t *var, FILE *fp);
void print1DFloat(vars_t *var, FILE *fp);
void print2DFloat(vars_t *var, FILE *fp);
void print3DFloat(vars_t *var, FILE *fp);
void printFloat(vars_t *var, FILE *fp);
void printString(vars_t *var, FILE *fp);
void print1DString(vars_t *var, FILE *fp);
void print2DString(vars_t *var, FILE *fp);
void print3DString(vars_t *var, FILE *fp);
void printGroup(vars_t *var, FILE *fp);

#define SIZEOF_PRINT_FUNCS 17

int print_function_dic_i[SIZEOF_PRINT_FUNCS] = {
        INTEGER,
        GROUP,
        ONEDINTEGER,
        TWODINTEGER,
        THREEDINTEGER,
        STRING,
        BOOL,
        ONEDBOOL,
        TWODBOOL,
        THREEDBOOL,
        FLOAT,
        ONEDFLOAT,
        TWODFLOAT,
        THREEDFLOAT,
        ONEDSTRING,
        TWODSTRING,
        THREEDSTRING
    };

void (*print_function_dic_v[SIZEOF_PRINT_FUNCS])(vars_t*, FILE*) = {
        printInteger,
        printGroup,
        print1DInteger,
        print2DInteger,
        print3DInteger,
        printString,
        printBoolean,
        print1DBoolean,
        print2DBoolean,
        print3DBoolean,
        printFloat,
        print1DFloat,
        print2DFloat,
        print3DFloat,
        print1DString,
        print2DString,
        print3DString
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
            fprintf(fp, "%d", ((int*)var->data)[(x*var->y_length)+y]);
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");

        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]\n");
}

void print3DInteger(vars_t *var, FILE *fp)
{
    int x, y, z, index;

    fprintf(fp, "[%s] = [", var->name);
    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "[");
            for(z=0; z < var->z_length; z++)
            {
                index = (z*var->x_length * var->y_length);
                index += (y*var->x_length) + x;
                fprintf(fp, "%d", ((int*)var->data)[index]);
                if(z+1 < var->z_length)
                    fprintf(fp, ", ");
            }
            fprintf(fp, "]");
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }

        fprintf(fp, "]");

        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]\n");
}

void printsingleBoolean(bool val, FILE *fp)
{
    if(val == 1)
        fprintf(fp, "true");
    else
        fprintf(fp, "false");
}

void printBoolean(vars_t *var, FILE *fp)
{
    fprintf(fp, "[%s] = [", var->name);
    printsingleBoolean(*((bool*)var->data), fp);
    fprintf(fp, "]\n");
}

void print1DBoolean(vars_t *var, FILE *fp)
{
    int x;

    fprintf(fp, "[%s] = [", var->name);
    for(x=0; x < var->x_length; x++)
    {
        printsingleBoolean(((bool*)var->data)[x], fp);
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]\n");
}

void print2DBoolean(vars_t *var, FILE *fp)
{
    int x, y, index;

    fprintf(fp, "[%s] = [", var->name);
    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            index = (x*var->y_length)+y;
            printsingleBoolean(((bool*)var->data)[x], fp);
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]\n");
}

void print3DBoolean(vars_t *var, FILE *fp)
{
    int x, y, z, index;

    fprintf(fp, "[%s] = [", var->name);
    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "[");
            for(z=0; z < var->z_length; z++)
            {
                index = (z*var->x_length * var->y_length);
                index += (y*var->x_length) + x;
                printsingleBoolean(((bool*)var->data)[index], fp);
                if(z+1 < var->z_length)
                   fprintf(fp, ", ");
            }
            fprintf(fp, "]");

            if(y+1 < var->y_length)
            fprintf(fp, ", ");
        }
        fprintf(fp, "]");
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]\n");
}

void printFloat(vars_t *var, FILE *fp)
{
    fprintf(fp, "[%s] = [%f]\n", var->name, *((double*)var->data));
}

void print1DFloat(vars_t *var, FILE *fp)
{
    int i;

    fprintf(fp, "[%s] = [", var->name);
    for(i=0; i < var->x_length; i++)
    {
        fprintf(fp, "%f", ((double*)var->data)[i]);
        if(i+1 < var->x_length)
        {
            fprintf(fp, ", ");
        }
    }
    fprintf(fp, "]\n");
}

void print2DFloat(vars_t *var, FILE *fp)
{
    int x, y;

    fprintf(fp, "[%s] = [", var->name);
    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "%f", ((double*)var->data)[(x*var->y_length)+y]);
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");

        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]\n");
}

void print3DFloat(vars_t *var, FILE *fp)
{
    int x, y, z, index;

    fprintf(fp, "[%s] = [", var->name);
    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "[");
            for(z=0; z < var->z_length; z++)
            {
                index = (z*var->x_length * var->y_length);
                index += (y*var->x_length) + x;
                fprintf(fp, "%f", ((double*)var->data)[index]);
                if(z+1 < var->z_length)
                    fprintf(fp, ", ");
            }
            fprintf(fp, "]");
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }

        fprintf(fp, "]");

        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]\n");
}



void printString(vars_t *var, FILE *fp)
{
    fprintf(fp, "[%s] = [%S]\n", var->name, (wchar_t*)var->data);
}

void print1DString(vars_t *var, FILE *fp)
{
    int x, index;

    fprintf(fp, "[%s] = [", var->name);
    for(x=0; x < var->x_length; x++)
    {
        index = ((var->length+1)*sizeof(wchar_t))*x;
        fprintf(fp, "\"%S\"", (wchar_t*)(var->data+index));
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]\n");
}

void print2DString(vars_t *var, FILE *fp)
{
    int x, y, index;

    fprintf(fp, "[%s] = [", var->name);
    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            index = (var->y_length*(var->length*sizeof(wchar_t)));
            index = index*(x)+((var->length*sizeof(wchar_t))*y);
            fprintf(fp, "\"%S\"", (wchar_t*)(var->data+index));
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]\n");
}

void print3DString(vars_t *var, FILE *fp)
{
    int x, y, z, index;

    fprintf(fp, "[%s] = [", var->name);
    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "[");
            for(z=0; z < var->z_length; z++)
            {
                index = ((z*var->x_length+x) * var->y_length+y) * (var->length*sizeof(wchar_t));
                if(z+1 < var->z_length)
                    fprintf(fp, ", ");
            }
            fprintf(fp, "]");
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
