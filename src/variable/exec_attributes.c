#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "variable.h"
#include "attr_funcs.h"
#include "parser_utils.h"
#include "token.h"

char *attr_funcs[] = 
{
    "length"
};

int (*attr_function_dic[])(VariableObject*) = {
    len_func
};


int calculateOffset(int var_type, int index, int *length, int var_length);
int parseIndex(char *str, int line_no, int col_no);

//Dissolves every attribute and executes the function or read the value of the variable
//What ever happens the end result gets written into the data location and the variable 
//type gets written into the var_type entry
int execAttributes(VariableParseData *var_data)
{
    int number = var_data->number_of_attributes, offset = 0, i = 0, x = 0, 
        index = 0,
        array_length[6] = {0, 0, 0, 0, 0, 0},
        data_offset = 0,
        (*exec_func)(VariableObject*) = NULL;
    char *groupname = NULL, *varname = NULL;
    VariableObject *var = NULL,
                    *target = &var_data->target;

    void *data = NULL;


    /*if(isdigit_str(var_data->attributes[0].attribute))
    {
        var_data->target.type = INTEGER;
        var_data->target.data = malloc(sizeof(int));
        *((int*)var_data->target.data) =         return(0);
    }*/

    if(number == 1)
    {
        varname = var_data->attributes[0].attribute;
        offset = 1;
    }
    else if(number > 1)
    {
        if(var_data->attributes[1].type == VARNAME)
        {
            groupname = var_data->attributes[0].attribute;
            varname   = var_data->attributes[1].attribute;
            offset = 2;
        }
        else
        {
            varname = var_data->attributes[0].attribute;
            offset = 1;
        }
    }

    if((var = getVariable(var_anker, groupname, varname)) == NULL)
    {
        if(rh4n_errno == GROUP_NOT_DEFINED)
        {
            PRINT_UNKWON_GRP_ATTR(var_data->attributes[0]);
        }
        else if(rh4n_errno == VARIABLE_NOT_DEFINED)
        {
            PRINT_UNKWON_VAR_ATTR(var_data->attributes[offset-1]);
        }
        return(-1);
    }
    //printVarPtr(var, stdout);

    target->type = var->type;
    target->data = var->data;
    target->name = malloc(1);
    *target->name = '\0';
    target->length = var->length;

    SET_FLAG(target->flags, RH4N_FLG_COPY);

    array_length[0] = var->array_length[0];
    array_length[1] = var->array_length[1];
    array_length[2] = var->array_length[2];

    if(var_data->attributes[offset-1].index_type != 0)
    {
        for(i=0; i < var_data->attributes[offset-1].index_type; i++)
        {
            //TODO: parse index string with  getVariableAttributes
            //index = (int)strtol(var_data->attributes[offset-1].index[i], NULL, 10);

            index = parseIndex(var_data->attributes[offset-1].index[i],
                var_data->attributes[offset-1].line, 
                var_data->attributes[offset-1].start_col);
            if(index >= array_length[i])
            {
                fprintf(stderr, "Index out of range\n");
                return(-1);
            }

            data_offset = calculateOffset(target->type, index, &array_length[i], 
                target->length);

            if(translateVarTypeWithIndex(target->type, &target->type) < 0)
            {
                PRINT_UNSUPPORTED_INDEX_ATTR(var_data->attributes[offset-1]);
                return(-1);
            }
            target->data = target->data+data_offset;
            target->array_length[0] = array_length[i+1];
            target->array_length[1] = array_length[i+2];
            target->array_length[2] = array_length[i+3];

        }
    }
    //printVarPtr(target, stdout);

    for(i=offset; i < var_data->number_of_attributes; i++)
    {
        for(x=0; x < sizeof(attr_funcs)/sizeof(char*); x++)
        {
            if(strcmp(var_data->attributes[i].attribute, attr_funcs[x]) == 0)
            {
                exec_func = attr_function_dic[x];
            }
        }

        if(exec_func == NULL)
        {
            printf("Unkown function\n");
            return(-1);
        }
        if(exec_func(target) == UNSUPPOERTED_TYPE)
        {
            printf("UNSUPPOERTED_TYPE\n");
            return(-1);
        }
    }
    //printVarPtr(target, stdout);

    return(0);
}

int calculateOffset(int var_type, int index, int *length, int var_length)
{
    switch(var_type)
    {
        case THREEDINTEGER:
            return(OFFSET_3DInteger(length[0], length[1], length[2], index, 
                0, 0));
        case TWODINTEGER:
            return(OFFSET_2DInteger(length[1], index, 0));
        case ONEDINTEGER: 
            return(OFFSET_1DInteger(index));
        case INTEGER:
            return(0);
        case THREEDSTRING:
            return(OFFSET_3DString(var_length, length[1], length[2], index, 0,
                0));
        case TWODSTRING:
            return(OFFSET_2DString(var_length, length[1], index, 0));
        case ONEDSTRING:
            return(OFFSET_1DString(var_length, index));
        case STRING:
            return(0);
    }
    return(-1);
}

int translateVarTypeWithIndex(int input_type, int *output)
{
    switch(input_type)
    {
        case THREEDINTEGER:
            *output = TWODINTEGER;
            break;
        case TWODINTEGER:
            *output = ONEDINTEGER;
            break;
        case ONEDINTEGER:
            *output = INTEGER;
            break;
        case INTEGER:
            return(-1);
        case THREEDSTRING:
            *output = TWODSTRING;
            break;
        case TWODSTRING:
            *output = ONEDSTRING;
            break;
        case ONEDSTRING:
            *output = STRING;
            break;
        case STRING:
            return(-1);
        case THREEDBOOL:
            *output = TWODBOOL;
            break;
        case TWODBOOL:
            *output = ONEDBOOL;
            break;
        case ONEDBOOL:
            *output = BOOL;
            break;
        case BOOL:
            return(-1);
        case THREEDFLOAT:
            *output = TWODFLOAT;
            break;
        case TWODFLOAT:
            *output = ONEDFLOAT;
            break;
        case ONEDFLOAT:
            *output = FLOAT;
            break;
        case FLOAT:
            return(-1);
    }
    return(0);
}

int parseIndex(char *str, int line_no, int col_no)
{
    VariableParseData var_data = 
    {
        {NULL, -1, NULL, 0, {0, 0, 0}, 0, 0, NULL, NULL, NULL}, -1, NULL
    };

    Token_Object anker = { -1, L'\0', -1, -1, NULL },
                 *cur = &anker;

    wchar_t buff = L'\0';
    int i = 0;

    if(isdigit_str(str))
    {
        return((int)strtol(str,
            NULL, 10));
    }

    for(; i < strlen(str); i++)
    {
        mbtowc(&buff, &str[i], 1);

        cur = safeToken(cur, buff, line_no, col_no+i);
    }

    getVariableAttributes(anker.next, &var_data);

    execAttributes(&var_data);

    //printVarPtr(&var_data.target, PRINT_MODE_FORMAT, stdout);
    
    cleanTokenList(anker.next);
    freeVariableData(&var_data);
    return(*((int*)var_data.target.data));
}
