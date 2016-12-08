#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "parser.h"
#include "exec_if.h"

extern int parseLine(char *line, status_t *status);

#define SIZE_OF_COMPARE_TABLE 6
int compare_table[SIZE_OF_COMPARE_TABLE][2] =
{
    {STRING, STRING},
    {FLOAT, INTEGER},
    {FLOAT, FLOAT},
    {INTEGER, INTEGER},
    {INTEGER, FLOAT},
    {BOOL, BOOL}
    
};
/*
 * return code: > 0 Var type
 *               -1 Unsupported
 */
int if_getVarType(if_parms_t *parm)
{
    int ret = 0;

    if(parm->type == IFSTR)
        return(STRING);

    if((ret = getVarType(vars_anker, NULL, parm->val)) < 0)
    {
        return(-1);
    }

    if(parm->hasindex == 1 && ret == TWODSTRING)
        return(ONEDSTRING);
    else if(parm->hasindex == 1 && ret == ONEDSTRING)
        return(STRING);
    else if(parm->hasindex == 1 && ret == STRING)
        return(STRING);
    else if(parm->hasindex == 2 && ret == TWODSTRING)
        return(STRING);
    else if(parm->hasindex == 2 && ret == ONEDSTRING)
        return(STRING);
    else if(parm->hasindex == 2 && ret == STRING)
        return(-1);

    if(parm->hasindex == 1 && ret == TWODINTEGER)
        return(ONEDINTEGER);
    else if(parm->hasindex == 1 && ret == ONEDINTEGER)
        return(INTEGER);
    else if(parm->hasindex == 1 && ret == INTEGER)
        return(-1);
    else if(parm->hasindex == 2 && ret == TWODINTEGER)
        return(INTEGER);
    else if(parm->hasindex == 2 && ret == ONEDINTEGER)
        return(-1);
    else if(parm->hasindex == 2 && ret == INTEGER)
        return(-1);

    if(parm->hasindex == 1 && ret == TWODFLOAT)
        return(ONEDFLOAT);
    else if(parm->hasindex == 1 && ret == ONEDFLOAT)
        return(FLOAT);
    else if(parm->hasindex == 1 && ret == FLOAT)
        return(-1);
    else if(parm->hasindex == 2 && ret == TWODFLOAT)
        return(FLOAT);
    else if(parm->hasindex == 2 && ret == ONEDFLOAT)
        return(-1);
    else if(parm->hasindex == 2 && ret == FLOAT)
        return(-1);

    if(parm->hasindex == 1 && ret == TWODBOOL)
        return(ONEDBOOL);
    else if(parm->hasindex == 1 && ret == ONEDBOOL)
        return(BOOL);
    else if(parm->hasindex == 1 && ret == BOOL)
        return(-1);
    else if(parm->hasindex == 2 && ret == TWODBOOL)
        return(BOOL);
    else if(parm->hasindex == 2 && ret == ONEDBOOL)
        return(-1);
    else if(parm->hasindex == 2 && ret == BOOL)
        return(-1);
}

int exec_if(if_parms_t *parms, char **body, int body_length)
{
    int i;
    status_t status;  

    status.in_for = 0;
    status.in_if = 0;
    status.save_buff= NULL;
    status.sizeof_sav_buff = 0;

    printf("type: [%d]\n", if_getVarType(parms->next));

    printf("---------If body-------\n");
    for(i=1; i < body_length; i++)
    {
        //parseLine(body[i], &status);
    }
    printf("---------end-if--------\n");
    freeLineBuff(&status);
    return(0);
}
