#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "if.h"

int if_handling(token_t *anker, status_t *stat)
{
    printf("Hallo ich bin ein if\n");
    if(stat->just_save == true)
    {
        stat->in_if++;
        return(JUSTSAVE);
    }

    stat->just_save = true;
    stat->in_if = 1;
    stat->lookfor = IFBLOCK;
    return(JUSTSAVE);
}

int end_if_handling(token_t *anker, status_t *stat)
{
    int i;

    if(stat->in_if == 0)
    {
        fprintf(stderr, "Found end-if without starting for\n");
        return(-1);
    }
    if(--stat->in_if != 0)
    {
        printf("Ende eines if blocks erreicht\n");
        return(JUSTSAVE);
    }
    if(stat->lookfor != IFBLOCK)
    {
        printf("Ende eines if blocks erreicht\n");
        return(JUSTSAVE);
    }
    printf("OK fuere nun if aus\n");
    printf("---------If body-------\n");
    for(i=1; i < stat->sizeof_sav_buff; i++)
    {
        printf("%s\n", stat->save_buff[i]);
    }
    printf("-----------------------\n");

    stat->just_save = false;
    stat->lookfor = 0;

    freeLineBuff(stat);
    return(0);
}
