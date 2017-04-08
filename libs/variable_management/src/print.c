#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "var_management.h"

int printGroup(VariableObject *target, FILE *output, int format);

int print_function_types[] = {
    STRING,
    ONEDSTRING,
    TWODSTRING,
    THREEDSTRING,
    INTEGER,
    ONEDINTEGER,
    TWODINTEGER,
    THREEDINTEGER,
    BOOL,
    ONEDBOOL,
    TWODBOOL,
    THREEDBOOL,
    FLOAT,
    ONEDFLOAT,
    TWODFLOAT,
    THREEDFLOAT,
    GROUP
    };

int (*print_functions[])(VariableObject*, FILE*, int) = {
    printString,
    print1DString,
    print2DString,
    print3DString,
    printInteger,
    print1DInteger,
    print2DInteger,
    print3DInteger,
    printBoolean,
    print1DBoolean,
    print2DBoolean,
    print3DBoolean,
    printFloat,
    print1DFloat,
    print2DFloat,
    print3DFloat,
    printGroup
    };

int printGroup(VariableObject *target, FILE *output, int format)
{
    VariableObject *hptr = NULL;
    int i = 0, tabs = 0;
    int (*print_function)(VariableObject *, FILE*, int) = NULL;

    hptr = target->next_lvl;

    if(format == PRINT_MODE_FORMAT)
        fprintf(output, "[%s]:\n", target->name);
    else if(format == PRINT_MODE_JSON)
        fprintf(output, "\"%s\": {", target->name);
    else if(format == PRINT_MODE_RAW)
    {
        fprintf(output, "Group [%s]\n", target->name);
        return(0);
    }

    while(hptr)
    {
        for(i=0; i < (sizeof(print_function_types)/sizeof(int)); i++)
        {
            if(print_function_types[i] == hptr->type)
            {
                print_function = print_functions[i];
                break;
            }
        }
        if(print_function)
        {
            if(format ==  PRINT_MODE_FORMAT)
                fprintf(output, "\t");
            print_function(hptr, output, format);
        }
        hptr = hptr->next;
    }

    if(format == PRINT_MODE_JSON)
        fprintf(output, "}");
}

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
        }
        if(hptr->next)
            fprintf(output, ",");
        print_function = NULL;
        hptr = hptr->next;
    }
}

void printVarPtr(VariableObject *target, int format, FILE *output)
{
    int i = 0;

    int (*print_function)(VariableObject *, FILE*, int) = NULL;

    for(i=0; i < (sizeof(print_function_types)/sizeof(int)); i++)
    {
        if(print_function_types[i] == target->type)
            print_function = print_functions[i];
    }
    
    if(print_function)
    {
        print_function(target, output, format);
        //fprintf(output, "\n");
    }
}

int printVar(VariableObject *anker, char *group, char *name, FILE *output)
{
    int i = 0;

    int (*print_function)(VariableObject *, FILE*, int) = NULL;

    VariableObject *target = NULL;

    if((target = getVariable(anker, group, name)) == NULL)
    {
        return(-1);
    }

    for(i=0; i < (sizeof(print_function_types)/sizeof(int)); i++)
    {
        if(print_function_types[i] == target->type)
            print_function = print_functions[i];
    }
    
    if(print_function)
    {
        print_function(target, output, PRINT_MODE_FORMAT);
        fprintf(output, "\n");
    }
    return(0);
}
