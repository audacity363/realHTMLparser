#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "var_management.h"

int print_function_types[] = {
    STRING,
    ONEDSTRING,
    TWODSTRING,
    THREEDSTRING
    };

int (*print_functions[])(VariableObject*, FILE*, int) = {
    printString,
    print1DString,
    print2DString,
    print3DString
    };

int printAllVars(VariableObject *anker, FILE *output)
{
    VariableObject *hptr = anker;
    int i = 0;

    int (*print_function)(VariableObject *, FILE*, int) = NULL;

    while(hptr)
    {
        if(hptr->type == -1)
        {
            hptr = hptr->next;
            continue;
        }

        for(i=0; i < (sizeof(print_function_types)/sizeof(int)); i++)
        {
            if(print_function_types[i] == hptr->type)
                print_function = print_functions[i];
        }
        
        if(print_function)
        {
            print_function(hptr, output, PRINT_MODE_FORMAT);
            fprintf(output, "\n");
        }
        print_function = NULL;
        hptr = hptr->next;
    }
}

int printAllVarsJson(VariableObject *anker, FILE *output)
{
    VariableObject *hptr = anker;
    int i = 0;

    int (*print_function)(VariableObject *, FILE*, int) = NULL;

    while(hptr)
    {
        if(hptr->type == -1)
        {
            hptr = hptr->next;
            continue;
        }

        for(i=0; i < (sizeof(print_function_types)/sizeof(int)); i++)
        {
            if(print_function_types[i] == hptr->type)
                print_function = print_functions[i];
        }
        
        if(print_function)
        {
            print_function(hptr, output, PRINT_MODE_JSON);
            fprintf(output, "\n");
        }
        print_function = NULL;
        hptr = hptr->next;
    }
}
