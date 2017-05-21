#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>

#include "parser.h"
#include "jsmn.h"
#include "json.h"

int convertToInt(char *str)
{
    long val = 0;
    val = strtol(str, NULL, 10);
    return((int)val);
}

double convertToDouble(char *str)
{
    double val = 0;
    val = strtod(str, NULL);
    return(val);
}

bool convertToBool(char *str)
{
    if(str[0] == 't')
        return(true);
    return(false);
}

int jsoneq(char *json, jsmntok_t *tok, const char *s) 
{
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int loadJson(char *filepath)
{
    char *json_str = NULL, varname[32];
    int filesize = 0, elements = 0, i=0;

    FILE *jsonfile = NULL;

    jsmn_parser jsparser;
    jsmntok_t jstokens[1024];

    if((jsonfile = fopen(filepath, "r")) == NULL)
    {
        fprintf(stderr, "Error while opening file: [%s] (%s)\n", filepath, strerror(errno));
        return(-1);
    }

    fseek(jsonfile, 0L, SEEK_END);
    filesize = ftell(jsonfile);
    fseek(jsonfile, 0L, SEEK_SET);

    if((json_str = malloc((filesize+1)*sizeof(char))) == NULL)
    {
        fprintf(stderr, "Error while allocating memory for json (%s)\n", strerror(errno));
        return(-1);
    }

    fread(json_str, filesize, 1, jsonfile);
    json_str[filesize-1] = '\0';

    printf("json: [%s]\n", json_str);

    jsmn_init(&jsparser);
    elements = jsmn_parse(&jsparser, json_str, strlen(json_str), jstokens, 
        sizeof(jstokens)/sizeof(jstokens[0]));

    if(elements < 0)
    {
        fprintf(stderr, "Failed to parse json: %d\nJSON: [%s]\n", elements, json_str);
        free(json_str);
        return(-1);
    }

    if(elements < 2)
    {
        fprintf(stderr, "Gotten an empty JSON object\n");
        free(json_str);
        return(-1);
    }
    if(jstokens[0].type != JSMN_OBJECT && jstokens[1].type != JSMN_ARRAY)
    {
        fprintf(stderr, "Expected a JSON Array within a JSON Object\n");
        free(json_str);
        return(-1);
    }
    
    for(i=3; i < elements; i++)
    {
        if(jstokens[i].type == JSMN_OBJECT)
        {
            i = i+parseVarObject(json_str, jstokens, i);
        }
        else
        {
            fprintf(stderr, "Unexpected Token\n");
            break;
        }
    }
    
    free(json_str);

    fclose(jsonfile);
    return(0);
}


int parseVarObject(char *json_str, jsmntok_t *tokens, int offset)
{
    int i, entries = 0;
    char *varname, *value;

    for(i=1; i < tokens[offset].size*2; i++)
    {
        if(jsoneq(json_str, &tokens[offset+i], "name") == 0)
        {
            /*printf("- Varname: %.*s\n", tokens[offset+i+1].end-tokens[offset+i+1].start,
					json_str + tokens[offset+i+1].start);*/
            varname = strndup(json_str+tokens[offset+i+1].start, tokens[offset+i+1].end-tokens[offset+i+1].start);
            printf("Varname: %s\n", varname);
            entries=entries+2;
            i++;
        }
		else if (jsoneq(json_str, &tokens[offset+i], "value") == 0) {
			printf("- Var value: %.*s\n", tokens[offset+i+1].end-tokens[offset+i+1].start,
					json_str + tokens[offset+i+1].start);
            value = strndup(json_str+tokens[offset+i+1].start, tokens[offset+i+1].end-tokens[offset+i+1].start);
            entries++;
            if(tokens[offset+i+1].type == JSMN_STRING)
            {
                entries++;
                handleJsonString_STRING(varname, value);
            }
            else if(tokens[offset+i+1].type == JSMN_PRIMITIVE)
            {
                entries++;
                handleJsonString_PRIMITIVE(varname, value);
            }
            else if(tokens[offset+i+1].type == JSMN_ARRAY)
            {
                entries = entries + handleJsonArray(varname, tokens, offset+i+1, json_str);
            }
            i++;
        }
    }
    free(varname);
    return(entries);
}



int handleJsonString_STRING(char *varname, char *value)
{
    wchar_t *wc_value = NULL;

    wc_value = malloc((strlen(value)+1)*SIZEOF_WCHAR);

    mbstowcs(wc_value, value, strlen(value)+1);

    newString(var_anker, NULL, varname, strlen(value));
    setString(var_anker, NULL, varname, wc_value);

    free(wc_value);
}

int handleJsonString_PRIMITIVE(char *varname, char *value)
{
    double d_value = 0;
    int    i_value = 0;

    if(value[0] == 't')
    {
        newBoolean(var_anker, NULL, varname);
        setBoolean(var_anker, NULL, varname, true);
        return(0);
    }
    else if(value[1] == 'f')
    {
        newBoolean(var_anker, NULL, varname);
        setBoolean(var_anker, NULL, varname, false);
        return(0);
    }
    else if(strchr(value, '.') != NULL)
    {
        d_value = convertToDouble(value);
        newFloat(var_anker, NULL, varname);
        setFloat(var_anker, NULL, varname, d_value);
    }
    else
    {
        i_value = convertToInt(value);
        newInteger(var_anker, NULL, varname);
        setInteger(var_anker, NULL, varname, i_value);
    }

}
