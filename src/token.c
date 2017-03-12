#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "parser.h"
#include "token.h"

Token_Object *safeToken(Token_Object *last, wchar_t chr, int line_no, int col_no)
{
    last->next = malloc(sizeof(Token_Object));
    last = last->next;

    last->line_no = line_no;
    last->col_no = col_no;
    if(chr != '\n')
        last->val = chr;
    else
        last->val = L' ';

    switch(chr)
    {
        case L'%':
            last->type = COMMANDBLOCK;
            break;
        case L'{':
            last->type = VARIABLEBLOCK;
            break;
        case L'}':
            last->type = BLOCKEND;
            break;
        case L' ':
        case L'\t':
        case L'\n':
            last->type = SPACE;
            break;
        case L'(':
            last->type = OPENBRACKET;
            break;
        case L')':
            last->type = CLOSEBRACKET;
            break;
        case L',':
            last->type = COMMA;
            break;
        case L'[':
            last->type = OPENCORNERBRACKET;
            break;
        case L']':
            last->type = CLOSECORNERBRACKET;
            break;
        case L'"':
        case L'\'':
            last->type = QUOTATIONMARKS;
            break;
        case L'\\':
            last->type = BACKSLASH;
            break;
        default:
            last->type = CHAR;
    }
    
    last->next = NULL;
    return(last);
}

void printfromTree(Token_Object *tree)
{
    Token_Object *hptr = tree;

    while(hptr)
    {
        printf("%C", hptr->val);
        hptr = hptr->next;
    }
}

void cleanTokenList(Token_Object *first)
{
    Token_Object *next = NULL, *hptr = NULL;

    //There is nothing in the list
    if(!first)
        return;

    next = first;

    while(1)
    {
        hptr = next->next;
        free(next);

        next = hptr;
        if(!next)
            break;
    }

    //first->next = NULL;
}

void printTokens(Token_Object *first)
{
    Token_Object *hptr = first->next;

    while(hptr)
    {
        switch(hptr->type)
        {
            case COMMANDBLOCK:
                printf("COMMANDBLOCK");
                break;
            case VARIABLEBLOCK:
                printf("VARIABLEBLOCK");
                break;
            case BLOCKEND:
                printf("BLOCKEND");
                break;
            case SPACE:
                printf("SPACE");
                break;
            case OPENBRACKET:
                printf("OPENBRACKET");
                break;
            case CLOSEBRACKET:
                printf("CLOSEBRACKET");
                break;
            case COMMA:
                printf("COMMA");
                break;
            case OPENCORNERBRACKET:
                printf("OPENCORNERBARCKET");
                break;
            case CLOSECORNERBRACKET:
                printf("CLOSECORNERBARCKET");
                break;
            case QUOTATIONMARKS:
                printf("QUOTATIONMARKS");
                break;
            case BACKSLASH:
                printf("BACKSLASH");
                break;
            case CHAR:
                printf("CHAR");
                break;
            default:
                printf("!!UNKOWN!!");
        }
        printf("\n");
        printf("\tval: [%C]\n", hptr->val);
        hptr = hptr->next;
    }
}
