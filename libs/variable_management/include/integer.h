#ifndef RH4N_VARIABLE_MAN_INTEGER
#define RH4N_VARIABLE_MAN_INTEGER
int newInteger(VariableObject *anker, char *group, char *name);
int setInteger(VariableObject *anker, char *group, char *name, int val);
int getInteger(VariableObject *anker, char *group, char *name);
int printInteger(VariableObject *target, FILE *output, int mode);
#endif
