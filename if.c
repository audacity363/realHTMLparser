#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "token_handling.h"
#include "command_parsing.h"
#include "if.h"
#include "exec_if.h"

int if_handling(token_t *anker, status_t *stat)
{
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

int parseIfHead(token_t *head, char *line)
{
    int line_length = strlen(line);

    return(lineToTokens(head, line, line+strlen(line)));
}

token_t *jumpOverCMDName(token_t *anker)
{
    token_t *hptr = anker->next;
    int found_cmd = false;

    while(hptr)
    {
        if(hptr->type == SPACE && found_cmd)
        {
            hptr = hptr->next;
            break;
        }
        else if(hptr->type == CHAR)
        {
            found_cmd = true;
        }
        hptr = hptr->next;
    }

    return(hptr);
}

int addParm(if_parms_t *parms, int val_len, char *val, int str, int compare)
{
    int type = -1;
    if_parms_t *new,
               *hptr = parms;

    if(str)
    {
        type = STR;
    }
    else if(compare)
    {
        type = COMPARE;
    }
    else if(!str && !compare)
    {
        type = VARIABLE;
    }

    while(hptr->next)
        hptr = hptr->next;

    if((new = malloc(sizeof(if_parms_t))) == NULL)
        return(-1);
    new->type = type;
    new->val_length = val_len;
    if((new->val = malloc(val_len)) == NULL)
    {
        free(new);
        return(-2);
    }    
    strcpy(new->val, val);
    new->next = NULL;
    new->prev = hptr;

    hptr->next = new;

    return(0);

}

void printParms(if_parms_t *anker)
{
    if_parms_t *hptr;

    if(anker->prev)
        hptr = anker;
    else
        hptr = anker->next;

    while(hptr)
    {
        switch(hptr->type)
        {
            case VARIABLE:
                printf("variable\n\tval: [%s]\n", hptr->val);
                break;
            case STR:
                printf("string\n\tval: [%s]\n", hptr->val);
                break;
            case COMPARE:
                printf("compare\n\tval: [%s]\n", hptr->val);
                break;
            default:
                printf("unkown\n\tval: [%s]\n", hptr->val);
                
        }
        hptr = hptr->next;
    }
}

token_t *findNextParm(if_parms_t *parms, token_t *tokens)
{
    token_t *hptr = tokens;
    int in_str = false,
        length = 1,
        type = -1,
        found_str = false,
        found_compare = false;
    char *buff = malloc(1);
    

    while(hptr)
    {
        if(hptr->type == SPACE && in_str)
        {
            buff[length-1] = hptr->val;
            buff = realloc(buff, ++length);
        }
        else if(hptr->type == SPACE)
            break;
        else if(hptr->type == STRING && in_str)
        {
            in_str = false;
            break;
        }
        else if(hptr->type == STRING && !in_str)
        {
            in_str = true;
            found_str = true;
        }
        else if(hptr->type == CHAR)
        {
            buff[length-1] = hptr->val;
            buff = realloc(buff, ++length);
        }
        else if(hptr->type == EQUALS || hptr->type == GREATERTHEN || hptr->type == LESSTHEN)
        {
            found_compare = true;
            buff[length-1] = hptr->val;
            buff = realloc(buff, ++length);
        }
        hptr = hptr->next;
    }
    if(!hptr || length == 1)
    {
        free(buff);
        return(NULL);
    }
    hptr = hptr->next;

    buff[length-1] = '\0';

    addParm(parms, length, buff, found_str, found_compare);

    free(buff);

    return(hptr);
}

token_t *findParms(if_parms_t *parms, token_t *head)
{
    token_t *hptr;
    int in_str = false,
        found_cmd = false;

    //Jump over command name
    hptr = jumpOverCMDName(head);
    
    return(hptr);
}


int end_if_handling(token_t *anker, status_t *stat)
{
    int i;
    char *if_head;

    token_t head = {' ', -1, NULL, NULL},
            *parm_start,
            *last_parm;
    if_parms_t parms = {NULL, -1, -1, NULL, NULL};

    if(stat->in_if == 0)
    {
        fprintf(stderr, "Found end-if without starting for\n");
        return(-1);
    }
    if(--stat->in_if != 0)
    {
        return(JUSTSAVE);
    }
    if(stat->lookfor != IFBLOCK)
    {
        return(JUSTSAVE);
    }

    if_head = stat->save_buff[0];
    parseIfHead(&head, if_head);

    parm_start = findParms(&parms, &head);

    last_parm = findNextParm(&parms, parm_start);
    while(last_parm)
    {
        last_parm = findNextParm(&parms, last_parm);
    }

    printParms(&parms);

    stat->just_save = false;
    stat->lookfor = 0;

    exec_if(&parms, stat->save_buff, stat->sizeof_sav_buff);

    freeLineBuff(stat);
    return(0);
}