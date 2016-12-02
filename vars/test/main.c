#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"

int main()
{
    vars_t anker;
    int ret, i, x, y, z;
    anker.name = NULL;
    anker.prev = NULL;
    anker.next = NULL;

    if((ret = addGroup(&anker, "hallo")) != 0)
    {
        printf("Error: [%s]\n", var_errorstrs[ret]);
        return(1);
    }

    addInteger(&anker, "hallo", "test", 677);
    addInteger(&anker, NULL, "test1", 67);
    addInteger(&anker, "hallo", "test2", 32);

    add1DIntegerArray(&anker, "hallo", "array", 10);
    for(i=0; i < 10; i++)
    {

        edit1DIntegerArray(&anker, "hallo", "array", i, i);
    }

    add2DIntegerArray(&anker, NULL, "2darray", 5, 7);

    z = 0;
    for(i=0; i < 5; i++)
    {
        for(x=0; x < 7; x++)
        {
            edit2DIntegerArray(&anker, NULL, "2darray", z, i, x);
            z++;
        }
    }

    printAllVars(&anker);
}
