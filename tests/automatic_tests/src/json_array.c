#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>

#include "parser.h"
#include "jsmn.h"
#include "json.h"

double convertToDouble1(char *str)
{
    double val = 0;
    val = strtod(str, NULL);
    //printf("Str: [(%p)%s]\t[%f]\n", str, str, val);

    return(val);
}

int getVariableType(char *str)
{
    if(strcmp(str, "null") == 0)
    {
        fprintf(stderr, "Null ist not supported\n");
        return(-1);
    }
    if(str[0] == 'f' || str[0] == 't')
        return(BOOL);
    else if(strchr(str, '.') != NULL)
        return(FLOAT);
    else
        return(INTEGER);
}

int handleOneDArray(char *varname, jsmntok_t *tokens, int offset, char *json_str)
{
    int i = 0, value_length = -1, max_length = 0,
        vartype = -1, i_val = 0;

    bool b_val = false;
    double d_val = 0, d_help = 0;

    jsmntok_t *start = NULL;

    char **values = NULL;
    wchar_t *wc_value = NULL;

    if(tokens[offset+1].type == JSMN_STRING)
        vartype = ONEDSTRING;

    values = malloc(tokens[offset].size*sizeof(char*));
    value_length = tokens[offset].size;

    for(i=1; i < tokens[offset].size+1; i++)
    {
        values[i-1] = strndup(json_str+tokens[offset+i].start, tokens[offset+i].end - tokens[offset+i].start);
        //printf("%d, %d: %.*s\n", x, i, start[i].end - start[i].start, json_str+start[i].start);
        printf("%d: %s\n", i-1, values[i-1]);
    }

    if(vartype != ONEDSTRING)
    {
        switch(getVariableType(values[0]))
        {
            case BOOL:
                vartype = ONEDBOOL;
                break;
            case FLOAT:
                vartype = ONEDFLOAT;
                break;
            case INTEGER:
                vartype = ONEDINTEGER;
                break;
            case -1:
                return(-1);
        }
    }
    if(vartype == ONEDSTRING)
    {
        for(i=0; i < value_length; i++)
            if(strlen(values[i]) > max_length)
                max_length = strlen(values[i]);

        new1DString(var_anker, NULL, varname, max_length, value_length);

        wc_value = malloc(max_length*SIZEOF_WCHAR);

        for(i=0; i < value_length; i++)
        {
            mbstowcs(wc_value, values[i], strlen(values[i]));
            set1DStringX(var_anker, NULL, varname, i, wc_value);
            memset(wc_value, 0x00, value_length*SIZEOF_WCHAR);
        }
        free(wc_value);
    }
    else if(vartype == ONEDBOOL)
    {
        new1DBoolean(var_anker, NULL, varname, value_length);
        for(i=0; i < value_length; i++)
        {
            b_val = convertToBool(values[i]);
            set1DBooleanX(var_anker, NULL, varname, i, b_val);
        }
    }
    else if(vartype == ONEDINTEGER)
    {
        new1DInteger(var_anker, NULL, varname, value_length);
        for(i=0; i < value_length; i++)
        {
            i_val = convertToInt(values[i]);
            set1DBooleanX(var_anker, NULL, varname, i, i_val);
        }
    }
    if(vartype == ONEDFLOAT)
    {
        new1DFloat(var_anker, NULL, varname, value_length);
        for(i=0; i < value_length; i++)
        {
            d_val = convertToDouble(values[i]);
            set1DFloatX(var_anker, NULL, varname, i, d_val);
        }
    }
    
    for(i=0; i < value_length; i++)
    {
        free(values[i]);
    }
    free(values);
    return(tokens[offset].size+1);

}

int handleTwoDArray(char *varname, jsmntok_t *tokens, int offset, char *json_str)
{
    int i = 0, x = 0, y = 0, l_offset = 0, x_length = -1, *y_length = NULL,
        length_of_ylength = -1, max_length = 0, max_y = 0, vartype = -1, i_val = 0;

    jsmntok_t *start = NULL;

    char ***values = NULL;
    wchar_t *wc_value = NULL;

    bool b_val = false;
    double d_val = 0;

    if(tokens[offset+2].type == JSMN_STRING)
        vartype = ONEDSTRING;

    x_length = tokens[offset].size;
    values = malloc(x_length*sizeof(char*));
    y_length = malloc(x_length*sizeof(int));
    
    for(x=0; x < tokens[offset].size; x++)
    {
        start = &tokens[offset+l_offset+1];
        l_offset = l_offset + tokens[offset+l_offset+1].size + 1;

        y_length[x] = start->size;
        values[x] = malloc(y_length[x]*sizeof(char*));

        if(y_length[x] > max_y)
            max_y = y_length[x];

        for(i=1; i < start->size+1; i++)
        {
            values[x][i-1] = strndup(json_str+start[i].start, start[i].end - start[i].start);
            if(strlen(values[x][i-1]) > max_length)
                max_length = strlen(values[x][i-1]);

            printf("%d, %d: %s\n", x, i-1, values[x][i-1]);
        }
    }

    if(vartype != TWODSTRING)
    {
        switch(getVariableType(values[0][0]))
        {
            case BOOL:
                vartype = TWODBOOL;
                break;
            case FLOAT:
                vartype = TWODFLOAT;
                break;
            case INTEGER:
                vartype = TWODINTEGER;
                break;
            case -1:
                return(-1);
        }
    }
    if(vartype == TWODSTRING)
    {
        new2DString(var_anker, NULL, varname, max_length, x_length, max_y);

        wc_value = malloc(max_length*(max_length+1));

        for(x=0; x < x_length; x++)
            for(y=0; y < y_length[x]; y++)
            {
                mbstowcs(wc_value, values[x][y], strlen(values[x][y]));
                set2DStringXY(var_anker, NULL, varname, x, y, wc_value);
                memset(wc_value, 0x00, (max_length+1)*SIZEOF_WCHAR);
            }
        free(wc_value);
    }
    else if(vartype == TWODBOOL)
    {
        new2DBoolean(var_anker, NULL, varname, x_length, max_y);
        for(x=0; x < x_length; x++)
            for(y=0; y < y_length[x]; y++)
            {
                b_val = convertToBool(values[x][y]);
                set2DBooleanXY(var_anker, NULL, varname, x, y, b_val);
            }
    }
    else if(vartype == TWODINTEGER)
    {
        new2DInteger(var_anker, NULL, varname, x_length, max_y);
        for(x=0; x < x_length; x++)
            for(y=0; y < y_length[x]; y++)
            {
                i_val = convertToInt(values[x][y]);
                set2DIntegerXY(var_anker, NULL, varname, x, y, i_val);
            }

    }
    else if(vartype == TWODFLOAT)
    {
        new2DFloat(var_anker, NULL, varname, x_length, max_y);
        for(x=0; x < x_length; x++)
            for(y=0; y < y_length[x]; y++)
            {
                d_val = convertToDouble(values[x][y]);
                set2DFloatXY(var_anker, NULL, varname, x, y, d_val);
            }

    }

    for(x=0; x < x_length; x++)
    {
        for(y=0; y < y_length[x]; y++)
            free(values[x][y]);
        free(values[x]);
    }
    free(values);
    free(y_length);

    return(l_offset+1);
}

int handleThreeDArray(char *varname, jsmntok_t *tokens, int offset, char *json_str)
{
    int x = 0, y = 0, z = 0, l_offset = 0, y_offset = 1,
        xlength = -1, *ylength = NULL, **zlength = NULL, maxlength = 0,
        maxylength = 0, maxzlength = 0, vartype = -1, i_val = 0;

    jsmntok_t *xstart = NULL, *ystart = NULL;

    char *value = NULL,
         ****data = NULL; //three dimension array of char pointers
    wchar_t *wc_value = NULL;

    bool b_val = false;
    double d_val = 0;

    if(tokens[offset+3].type == JSMN_STRING)
        vartype = THREEDSTRING;

    xlength = tokens[offset].size;
    data = malloc(xlength*sizeof(char*));
    ylength = malloc(xlength*sizeof(int));
    zlength = malloc(xlength*sizeof(int*));

    data = malloc(xlength*sizeof(char*));

    for(x=0; x < tokens[offset].size; x++)
    {
        xstart = &tokens[offset+l_offset+1];
        l_offset++;
        
        ylength[x] = xstart->size;
        data[x] = malloc(ylength[x]*sizeof(char*));
        zlength[x] = malloc(ylength[x]*sizeof(int));

        data[x] = malloc(ylength[x]*sizeof(char*));

        if(ylength[x] > maxylength)
            maxylength = ylength[x];

        for(y=1; y < xstart->size+1; y++)
        {
            ystart = &xstart[y_offset];
            l_offset = l_offset+ystart->size+1;
            y_offset = 1;

            //zlength[y-1] = malloc(ystart->size);
            zlength[x][y-1] = ystart->size;

            data[x][y-1] = malloc(ystart->size*sizeof(char*));

            if(ystart->size > maxzlength)
                maxzlength = ystart->size;

            y_offset=y_offset+ystart->size+1;
            for(z=1; z < ystart->size+1; z++)
            {
                value = strndup(json_str+ystart[z].start, ystart[z].end - ystart[z].start);
                if(strlen(value) > maxlength)
                    maxlength = strlen(value);
                //printf("%d, %d, %d: %s\n", x, y-1, z-1, value);
                data[x][y-1][z-1] = value;
            }
        }
        y_offset = 1;
    }

    if(vartype != THREEDSTRING)
    {
        switch(getVariableType(data[0][0][0]))
        {
            case BOOL:
                vartype = THREEDBOOL;
                break;
            case FLOAT:
                vartype = THREEDFLOAT;
                break;
            case INTEGER:
                vartype = THREEDINTEGER;
                break;
            case -1:
                return(-1);
        }
    }
    if(vartype == THREEDSTRING)
    {
        new3DString(var_anker, NULL, varname, maxlength, xlength, maxylength, maxzlength);

        wc_value = malloc((maxlength+1)*SIZEOF_WCHAR);

        for(x=0; x < xlength; x++)
            for(y=0; y < ylength[x]; y++)
                for(z=0; z < zlength[x][y]; z++)
                {
                    memset(wc_value, 0x00, maxlength*SIZEOF_WCHAR);
                    mbstowcs(wc_value, data[x][y][z], strlen(data[x][y][z]));
                    set3DStringXYZ(var_anker, NULL, varname, x, y, z, wc_value);
                }
        
        free(wc_value);
    }
    else if(vartype == THREEDBOOL)
    {
        new3DBoolean(var_anker, NULL, varname, xlength, maxylength, maxzlength);
        for(x=0; x < xlength; x++)
            for(y=0; y < ylength[x]; y++)
                for(z=0; z < zlength[x][y]; z++)
                {
                    b_val = convertToBool(data[x][y][z]);
                    set3DBooleanXYZ(var_anker, NULL, varname, x, y, z, b_val);
                }
    }
    else if(vartype == THREEDINTEGER)
    {
        new3DInteger(var_anker, NULL, varname, xlength, maxylength, maxzlength);
        for(x=0; x < xlength; x++)
            for(y=0; y < ylength[x]; y++)
                for(z=0; z < zlength[x][y]; z++)
                {
                    i_val = convertToInt(data[x][y][z]);
                    set3DIntegerXYZ(var_anker, NULL, varname, x, y, z, i_val);
                }
    }
    else if(vartype == THREEDFLOAT)
    {
        new3DFloat(var_anker, NULL, varname, xlength, maxylength, maxzlength);
        for(x=0; x < xlength; x++)
            for(y=0; y < ylength[x]; y++)
                for(z=0; z < zlength[x][y]; z++)
                {
                    d_val = convertToDouble(data[x][y][z]);
                    set3DFloatXYZ(var_anker, NULL, varname, x, y, z, d_val);
                }

    }

    return(l_offset+1);
}

int handleJsonArray(char *varname, jsmntok_t *tokens, int offset, char *json_str)
{
    int i = 0, vartype = -1, max_length = 0, array_type = 0;
    char **value;
    wchar_t *wc_value = NULL;

    if(tokens[offset].type == JSMN_ARRAY && tokens[offset+1].type == JSMN_ARRAY && 
            tokens[offset+2].type == JSMN_ARRAY)
    {
        return(handleThreeDArray(varname, tokens, offset, json_str));
    }
    else if(tokens[offset].type == JSMN_ARRAY && tokens[offset+1].type == JSMN_ARRAY)
    {
        return(handleTwoDArray(varname, tokens, offset, json_str));
    }
    if(tokens[offset].type == JSMN_ARRAY)
    {
        return(handleOneDArray(varname, tokens, offset, json_str));
    }
}
