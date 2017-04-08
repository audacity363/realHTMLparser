#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "variable.h"

char *getIndexString(char *start, VariableAttribute *attr);
void printAttributes(VariableParseData *data);

int getVariableAttributes(Token_Object *start, VariableParseData *var_data)
{
    Token_Object *hptr = start;

    int found_seperator = 0, buffer_length = -1, line = 0, start_col = 0, in_index = 0;
    char *buffer = NULL;

    //Jump over leading spaces and block begins
    for(; hptr->next->type != CHAR; hptr=hptr->next);

    //Go through the complete list and split on every '.' and save the saved chars as a new attribute.
    //When a '()' is found the token attribute gets rated as a function

    line = hptr->line_no;
    start_col = hptr->col_no;
    while(hptr)
    {
        //Found a seperator
        if(hptr->val == L'.' && in_index == 0)
        {

            saveAttribute(var_data, buffer, buffer_length, line, start_col);
            free(buffer);
            buffer = NULL;
            buffer_length = -1;
            hptr=hptr->next;
            line = hptr->line_no; start_col = hptr->col_no;
            continue;
        }
        //ignore spaces
        else if(hptr->type == SPACE)
        {
            hptr=hptr->next;
            continue;
        }
        //Reaced the end of the block
        else if(hptr->type == BLOCKEND)
        {
            saveAttribute(var_data, buffer, buffer_length, line, start_col);
            free(buffer);
            buffer = NULL;
            buffer_length = -1;
            break;
        }
        else
        {
            //Ignore the String in the index clamps
            if(hptr->val == L'[')
                in_index++;
            else if(hptr->val == L']')
                in_index--;
            //Append the buffer with the new char
            if(CHECK_UNICODE_CHAR(hptr->val))
            {
                PRINT_SYNTAX_ERROR(hptr);
                free(buffer);
                return(-1);
            }

            if(buffer_length == -1)
            {
                buffer = malloc(SIZEOF_CHAR);
                buffer_length = 1;
            }
            else
            {
                buffer = realloc(buffer, SIZEOF_CHAR*(++buffer_length));
            }
            buffer[buffer_length-1] = (char)hptr->val;

        }
        hptr=hptr->next;
    }
    if(buffer != NULL)
    {
        saveAttribute(var_data, buffer, buffer_length, line, start_col);
        free(buffer);
    }
    int i=0;

    rateAttributes(var_data);

    //printAttributes(var_data);

    return(0);
}

void freeVariableData(VariableParseData *data)
{
    int i = 0, x = 0;


    if(data->number_of_attributes != -1)
    {
        for(; i < data->number_of_attributes; i++)
        {
            free(data->attributes[i].attribute);
            if(data->attributes[i].index_type != 0)
            {
                for(x=0; x < data->attributes[i].index_type; x++)
                    free(data->attributes[i].index[x]);
                free(data->attributes[i].index);
            }
        }
        free(data->attributes);
        data->attributes = NULL;
        data->number_of_attributes = -1;
    }

    if(!ISSET_FLAG(data->target.flags, RH4N_FLG_COPY))
    {
        free(data->target.data);
    }

    data->target.data = NULL;
    free(data->target.name);
    data->target.name = NULL;
    memset(data->target.array_length, 0x00, sizeof(data->target.array_length)); 
    data->target.length = 0;
    data->target.type = -1;
    data->target.next_lvl = NULL;
}

void printAttributes(VariableParseData *data)
{
    int i,x;


    printf("----Attributes----\n");
    for(i=0; i < data->number_of_attributes; i++)
    {
        printf("[%s]\n", data->attributes[i].attribute);
        if(data->attributes[i].index_type != -1)
        {
            for(x=0; x < data->attributes[i].index_type; x++)
            {
                printf("\t[%s]\n", data->attributes[i].index[x]);
            }
        }
    }
    printf("------------------\n");
}

int saveAttribute(VariableParseData *data, char *buffer, int buffer_length, int line, int col)
{
    char *function = NULL, *open_index = NULL,
         *target_str = NULL, *sav = NULL;

    if(data->number_of_attributes == -1)
    {
        data->attributes = malloc(sizeof(VariableAttribute));
        data->number_of_attributes = 1;
    }
    else
    {
        data->attributes = realloc(data->attributes, sizeof(VariableAttribute)*(++data->number_of_attributes));
    }

    //Copy the attribute value
    data->attributes[data->number_of_attributes-1].attribute = malloc((buffer_length+1)*SIZEOF_CHAR);
    memcpy(data->attributes[data->number_of_attributes-1].attribute, buffer, buffer_length);
    data->attributes[data->number_of_attributes-1].attribute[buffer_length+1] = '\0';

    target_str = data->attributes[data->number_of_attributes-1].attribute;

    //Save the start position of the attribute
    data->attributes[data->number_of_attributes-1].line = line;
    data->attributes[data->number_of_attributes-1].start_col = col;

    //Prepare the Index props
    data->attributes[data->number_of_attributes-1].index_type = -1;
    data->attributes[data->number_of_attributes-1].index = NULL;

    if((open_index = strchr(target_str, '[')) != NULL)
    {
        sav = getIndexString(open_index, &data->attributes[data->number_of_attributes-1]);
        *open_index = '\0';

        while(strlen(sav) > 0)
        {
            sav = getIndexString(sav, &data->attributes[data->number_of_attributes-1]);
        }
    }

    //Check if the attribute is function
    if((function = strchr(data->attributes[data->number_of_attributes-1].attribute, '(')) != NULL)
    {
        data->attributes[data->number_of_attributes-1].type = FUNCTION;
        function[0] = '\0';
    }
    else
    {
        //Until futher parsing the type is set to VARNAME
        data->attributes[data->number_of_attributes-1].type = VARNAME;
    }

    return(0);
}

//parses one Index String and saves it into the attribute struct
//Returns a pointer to the next char behind the close index clamp
char *getIndexString(char *start, VariableAttribute *attr)
{
    char *hptr = start+1;

    int in_index = 0, length = 0;

    while(1)
    {
        if(*hptr == '[')
            in_index++;
        else if(*hptr == ']' && in_index == 0)
            break;
        else if(*hptr == ']')
            in_index--;

        hptr++;
    }

    length = hptr-start;
    //printf("Index: [%.*s]\n", length, start);

    if(attr->index_type == -1)
    {
        attr->index = malloc(sizeof(char*));
        attr->index_type = 1;
    }
    else
    {
        attr->index = realloc(attr->index, (++attr->index_type)*sizeof(char*));
    }


    attr->index[attr->index_type-1] = malloc((length+1)*SIZEOF_CHAR);
    strncpy(attr->index[attr->index_type-1], start+1, length-1);

    return(hptr+1);
}

//Rates the attributes which was parsed by saveAttribute. It ignores the function ones and just
//determ wich attribute the grp name and wich one the variable name is. (When there are multiple 
//options)
int rateAttributes(VariableParseData *var_data)
{
    int number = var_data->number_of_attributes-1, offset = 0;

   //Only a function was given. Without any variable or group name
    if(var_data->attributes[0].type == FUNCTION)
    {
        PRINT_SYNTAX_ERROR_VAR_ATTR(var_data->attributes[0]);
        return(-1);
    }
    
    if(number == 1)
    {
        //It could be a variable name. Lets asume this for the moment. When the attributes get executed
        //be can determine if it is a variable or a group
        offset = 1;
    }
    else if(number > 1)
    {
        if(var_data->attributes[1].type != FUNCTION)
        {
            //the first attribute must be a group and the second one a variable name
            var_data->attributes[0].type = GRPNAME;
            var_data->attributes[1].type = VARNAME;
        }
        offset = 2;
    }

    //Check if the rest if the attributes are only functions

    for(; offset < number; offset++)
    {
        if(var_data->attributes[offset].type != FUNCTION)
        {
            PRINT_SYNTAX_ERROR_VAR_ATTR(var_data->attributes[offset]);
            return(-2);
        }
    }
    return(0);
}
