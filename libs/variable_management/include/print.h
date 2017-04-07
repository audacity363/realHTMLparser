#ifndef RH4N_VARIABLE_MAN_PRINT
#define RH4N_VARIABLE_MAN_PRINT
int printAllVars(VariableObject *anker, FILE *output);
int printAllVarsJson(VariableObject *anker, FILE *output);
void printVarPtr(VariableObject *target, FILE *output);
int printVar(VariableObject *anker, char *group, char *name, FILE *output);
#endif
