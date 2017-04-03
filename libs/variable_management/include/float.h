#ifndef RH4N_VARIABLE_MAN_FLOAT
#define RH4N_VARIABLE_MAN_FLOAT
int newFloat(VariableObject *anker, char *group, char *name);
int setFloat(VariableObject *anker, char *group, char *name, double val);
double getFloat(VariableObject *anker, char *group, char *name);
int printFloat(VariableObject *target, FILE *output, int mode);
#endif
