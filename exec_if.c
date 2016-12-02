#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "exec_if.h"

extern int parseLine(char *line, status_t *status);

int exec_if(if_parms_t *parms, char **body, int body_length)
{
    int i;
    status_t status;  

    status.in_for = 0;
    status.in_if = 0;
    status.save_buff= NULL;
    status.sizeof_sav_buff = 0;

    printf("---------If body-------\n");
    for(i=1; i < body_length; i++)
    {
        parseLine(body[i], &status);
    }
    printf("---------end-if--------\n");
    freeLineBuff(&status);
    return(0);
}
