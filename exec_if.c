#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "parser.h"
#include "exec_if.h"

extern int parseLine(char *line, status_t *status);

//String String Equals
int SSEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//String String Unequals
int SSUE(if_parms_t*, if_parms_t*, if_parms_t*);

//Float Integer Equals
int FIEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Integer Unequals
int FIUE(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Integer Less then
int FILT(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Integer Less then equals
int FILTEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Integer Greater then
int FIGT(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Integer Greater then equals
int FIGTEQ(if_parms_t*, if_parms_t*, if_parms_t*);

//Float Float Equals
int FFEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Float Unequals
int FFUE(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Float Less then
int FFLT(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Float Less then equals
int FFLTEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Float Greater then
int FFGT(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Float Greater then equals
int FFGTEQ(if_parms_t*, if_parms_t*, if_parms_t*);

//Integer Integer Equals
int IIEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Integer Unequals
int IIUE(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Integer Less then
int IILT(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Integer Less then equals
int IILTEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Integer Greater then
int IIGT(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Integer Greater then equals
int IIGTEQ(if_parms_t*, if_parms_t*, if_parms_t*);

//Integer Float Equals
int IFEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Float Unequals
int IFUE(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Float Less then
int IFLT(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Float Less then equals
int IFLTEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Float Greater then
int IFGT(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Float Greater then equals
int IFGTEQ(if_parms_t*, if_parms_t*, if_parms_t*);

//Bool Bool Equals
int BBEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Bool Bool Unequals
int BBUE(if_parms_t*, if_parms_t*, if_parms_t*);

//String OneDString in
int SODSIN(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer OneDInteger in
int IODIIN(if_parms_t*, if_parms_t*, if_parms_t*);
//Float OneDFloat in
int FODFIN(if_parms_t*, if_parms_t*, if_parms_t*);


#define SIZE_OF_FUNCTION_TABLE 31
int if_function_table[SIZE_OF_FUNCTION_TABLE][3] = 
{
    {STRING, STRING, IFEQUALS},
    {STRING, STRING, IFUNEQUALS},

    {FLOAT, INTEGER, IFEQUALS},
    {FLOAT, INTEGER, IFUNEQUALS},
    {FLOAT, INTEGER, IFLESSTHEN},
    {FLOAT, INTEGER, IFLESSTHENEQUAL},
    {FLOAT, INTEGER, IFGREATERTHEN},
    {FLOAT, INTEGER, IFGREATERTHENEQUALS},

    {FLOAT, FLOAT, IFEQUALS},
    {FLOAT, FLOAT, IFUNEQUALS},
    {FLOAT, FLOAT, IFLESSTHEN},
    {FLOAT, FLOAT, IFLESSTHENEQUAL},
    {FLOAT, FLOAT, IFGREATERTHEN},
    {FLOAT, FLOAT, IFGREATERTHENEQUALS},

    {INTEGER, INTEGER, IFEQUALS},
    {INTEGER, INTEGER, IFUNEQUALS},
    {INTEGER, INTEGER, IFLESSTHEN},
    {INTEGER, INTEGER, IFLESSTHENEQUAL},
    {INTEGER, INTEGER, IFGREATERTHEN},
    {INTEGER, INTEGER, IFGREATERTHENEQUALS},

    {INTEGER, FLOAT, IFEQUALS},
    {INTEGER, FLOAT, IFUNEQUALS},
    {INTEGER, FLOAT, IFLESSTHEN},
    {INTEGER, FLOAT, IFLESSTHENEQUAL},
    {INTEGER, FLOAT, IFGREATERTHEN},
    {INTEGER, FLOAT, IFGREATERTHENEQUALS},

    {BOOL, BOOL, IFEQUALS},
    {BOOL, BOOL, IFUNEQUALS},

    {STRING, ONEDSTRING, IFIN},
    
    {INTEGER, ONEDINTEGER, IFIN},

    {FLOAT, ONEDINTEGER, IFIN}
};

int (*if_function_table_v[SIZE_OF_FUNCTION_TABLE])(if_parms_t*, if_parms_t*, if_parms_t*) =
{
    SSEQ,
    SSUE,
    FIEQ,
    FIUE,
    FILT,
    FILTEQ,
    FIGT,
    FIGTEQ,
    FFEQ,
    FFUE,
    FFLT,
    FFLTEQ,
    FFGT,
    FFGTEQ,
    IIEQ,
    IIUE,
    IILT,
    IILTEQ,
    IIGT,
    IIGTEQ,
    IFEQ,
    IFUE,
    IFLT,
    IFLTEQ,
    IFGT,
    IFGTEQ,
    BBEQ,
    BBUE,
    SODSIN,
    IODIIN,
    FODFIN
};

#define SIZE_OF_COMPARE_TABLE 9
int compare_table[SIZE_OF_COMPARE_TABLE][2] =
{
    {STRING, STRING},
    {FLOAT, INTEGER},
    {FLOAT, FLOAT},
    {INTEGER, INTEGER},
    {INTEGER, FLOAT},
    {BOOL, BOOL},
    {STRING, ONEDSTRING},
    {FLOAT, ONEDFLOAT},
    {INTEGER, ONEDINTEGER}
    
};

#define OPERATOR_COUNT 7
int operator_table[SIZE_OF_COMPARE_TABLE][OPERATOR_COUNT] = 
{
    {IFEQUALS, IFUNEQUALS, -1, -1, -1, -1, -1},

    {IFEQUALS, IFUNEQUALS, IFLESSTHEN, 
     IFLESSTHENEQUAL, IFGREATERTHEN,
     IFGREATERTHENEQUALS, -1},

    {IFEQUALS, IFUNEQUALS, IFLESSTHEN, 
     IFLESSTHENEQUAL, IFGREATERTHEN,
     IFGREATERTHENEQUALS, -1},

    {IFEQUALS, IFUNEQUALS, IFLESSTHEN, 
     IFLESSTHENEQUAL, IFGREATERTHEN, 
     IFGREATERTHENEQUALS, -1},

    {IFEQUALS, IFUNEQUALS, IFLESSTHEN, 
     IFLESSTHENEQUAL, IFGREATERTHEN, 
     IFGREATERTHENEQUALS, -1},

    {IFEQUALS, IFUNEQUALS, -1, -1, -1, -1, -1},

    {IFIN, -1, -1, -1, -1, -1, -1}
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

    if(parm->hasindex == 0 && ret == STRING)
        return(STRING);
    else if(parm->hasindex == 0 && ret == ONEDSTRING)
        return(ONEDSTRING);
    else if(parm->hasindex == 0 && ret == TWODSTRING)
        return(-1);
    else if(parm->hasindex == 1 && ret == TWODSTRING)
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

    if(parm->hasindex == 0 && ret == INTEGER)
        return(INTEGER);
    else if(parm->hasindex == 0 && ret == ONEDINTEGER)
        return(ONEDINTEGER);
    else if(parm->hasindex == 0 && ret == TWODINTEGER)
        return(-1);
    else if(parm->hasindex == 1 && ret == TWODINTEGER)
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

    if(parm->hasindex == 0 && ret == FLOAT)
        return(FLOAT);
    else if(parm->hasindex == 0 && ret == ONEDFLOAT)
        return(ONEDFLOAT);
    else if(parm->hasindex == 0 && ret == TWODFLOAT)
        return(-1);
    else if(parm->hasindex == 1 && ret == TWODFLOAT)
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

    if(parm->hasindex == 0 && ret == BOOL)
        return(BOOL);
    else if(parm->hasindex == 0 && ret == ONEDBOOL)
        return(ONEDBOOL);
    else if(parm->hasindex == 0 && ret == TWODBOOL)
        return(-1);
    else if(parm->hasindex == 1 && ret == TWODBOOL)
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

    return(-2);
}

int checkIfcompatible(int types[3])
{
    int i = 0,
        x = 0;

    printf("type1[%d]\ntype2[%d]\ntype3[%d]\n", types[0], types[1], types[2]);

    for(; i < SIZE_OF_COMPARE_TABLE; i++)
    {
        if(compare_table[i][0] == types[0] &&
           compare_table[i][1] == types[2])
        {
            for(; x < OPERATOR_COUNT; x++)
            {
                if(types[1] == operator_table[i][x])
                    return(true);
            }
            return(false);
        }
    }
    return(false);
}

int (*getIfFunction(int types[3]))(if_parms_t*, if_parms_t*, if_parms_t*)
{
    int i = 0;

    for(; i < SIZE_OF_FUNCTION_TABLE; i++)
    {
        if(if_function_table[i][0] == types[0] &&
           if_function_table[i][1] == types[2] &&
           if_function_table[i][2] == types[1])
            return(if_function_table_v[i]);
    }
    return(NULL);
}

/*
 * return code: > 0 Compare type
 *               -1 Unsupported
 */
int getCompareType(char *val)
{
    if(strcmp(val, "==") == 0)
        return(IFEQUALS);
    if(strcmp(val, "<=") == 0)
        return(IFLESSTHENEQUAL);
    if(strcmp(val, ">=") == 0)
        return(IFGREATERTHENEQUALS);
    if(strcmp(val, "<") == 0)
        return(IFLESSTHEN);
    if(strcmp(val, ">") == 0)
        return(IFGREATERTHEN);
    if(strcmp(val, "!=") == 0)
        return(IFUNEQUALS);
    if(strcmp(val, "in") == 0)
        return(IFIN);

    return(-1);
}

/*
 * Currently the if just works with one query. The rest of the
 * head is ignored
 */
int exec_if(if_parms_t *parms, wchar_t **body, int body_length)
{
    int i = 0,
        types[3] = {-1, -1, -1},
        (*if_function)(if_parms_t*, if_parms_t*, if_parms_t*) = NULL;

    if_parms_t *leftval = NULL,
               *compare = NULL,
               *rightval = NULL;

    status_t status;  

    status.in_for = 0;
    status.in_if = 0;
    status.save_buff= NULL;
    status.sizeof_sav_buff = 0;

    //Check if the first three parms are set
    if(!parms->next)
       return(-1);
    if(!parms->next->next)
       return(-2);
    if(!parms->next->next->next)
       return(-3);

    leftval = parms->next; compare = parms->next->next; rightval = parms->next->next->next;

    if((types[0] = if_getVarType(leftval)) < 0)
    {
        //Unsuported Type in first parm
        printf("Unsupported Type in first parm\n");
        return(-4);
    }
    leftval->varlib_type = types[0];

    if((types[1] = getCompareType(compare->val)) < 0)
    {
        //Unsuported Type in compare parm
        printf("Unsupported Type in compare parm\n");
        return(-5);
    }
    compare->varlib_type = types[1];

    if((types[2] = if_getVarType(rightval)) < 0)
    {
        //Unsuported Type in second parm
        printf("Unsupported Type in second parm\n");
        return(-6);
    }
    rightval->varlib_type = types[2];

    if(checkIfcompatible(types) != true)
    {
        printf("Type are not compatible\n");
        exit(-1);
    }

    if((if_function = getIfFunction(types)) == NULL)
    {
        printf("Can not get If Function\n");
        return(-8);
    }

    if_function(leftval, compare, rightval);

    printf("---------If body-------\n");
    for(i=1; i < body_length; i++)
    {
        //parseLine(body[i], &status);
    }
    printf("---------end-if--------\n");
    freeLineBuff(&status);
    return(0);
}

//String String Equals
int SSEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}

//String String Unequals
int SSUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}

//Float Integer Equals
int FIEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Float Integer Unequals
int FIUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Float Integer Less then
int FILT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Float Integer Less then equals
int FILTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Float Integer Greater then
int FIGT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Float Integer Greater then equals
int FIGTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}

//Float Float Equals
int FFEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Float Float Unequals
int FFUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Float Float Less then
int FFLT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Float Float Less then equals
int FFLTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Float Float Greater then
int FFGT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Float Float Greater then equals
int FFGTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}

//Integer Integer Equals
int IIEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Integer Integer Unequals
int IIUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Integer Integer Less then
int IILT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Integer Integer Less then equals
int IILTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Integer Integer Greater then
int IIGT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Integer Integer Greater then equals
int IIGTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}

//Integer Float Equals
int IFEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Integer Float Unequals
int IFUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Integer Float Less then
int IFLT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Integer Float Less then equals
int IFLTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Integer Float Greater then
int IFGT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Integer Float Greater then equals
int IFGTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}

//Bool Bool Equals
int BBEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Bool Bool Unequals
int BBUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}

//String OneDString in
int SODSIN(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Integer OneDInteger in
int IODIIN(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}
//Float OneDFloat in
int FODFIN(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
}

