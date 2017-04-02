#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "var_management.h"

int main()
{
    VariableObject *anker = NULL;

    initAnker(&anker);

    newString(anker, NULL, "test1", 20);
    setString(anker, NULL, "test1", L"TEst");

    new1DString(anker, NULL, "test", 20, 4);
    set1DStringX(anker, NULL, "test", 0, L"Hello World");
    set1DStringX(anker, NULL, "test", 1, L"Hello World1");

    new2DString(anker, NULL, "test2", 20, 2, 3);

    new3DString(anker, NULL, "test3", 20, 2, 3, 4);

    newInteger(anker, NULL, "test4");
    setInteger(anker, NULL, "test4", 73527);

    new1DInteger(anker, NULL, "test5", 3);
    set1DIntegerX(anker, NULL, "test5", 2, 1);
    set1DIntegerX(anker, NULL, "test5", 0, 6);
    set1DIntegerX(anker, NULL, "test5", 1, 9);

    printf("0: [%S]\n", get1DStringX(anker, NULL, "test", 0));
    printf("1: [%S]\n", get1DStringX(anker, NULL, "test", 1));

    printAllVars(anker, stdout);
    printAllVarsJson(anker, stdout);
    return(0);
}
