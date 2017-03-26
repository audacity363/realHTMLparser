#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "variable.h"

//Dissolves every attribute and executes the function or read the value of the variable
//What ever happens the end result gets written into the data location and the variable 
//type gets written into the var_type entry
int execAttributes(VariableParseData *var_data)
{
    int number = var_data->number_of_attributes, offset = 0;
    char *groupname = NULL, *varname = NULL;

    if(number == 1)
    {
        varname = var_data->attributes[0].attribute;
    }
    else if(number > 1)
    {
        if(var_data->attributes[1].type == VARNAME)
        {
            groupname = var_data->attributes[0].attribute;
            varname   = var_data->attributes[1].attribute;
        }
        else
        {
            varname = var_data->attributes[0].attribute;
        }
    }
}
