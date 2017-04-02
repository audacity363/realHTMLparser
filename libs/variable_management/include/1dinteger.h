#ifndef RH4N_VARIABLE_MAN_1DINTEGER
#define RH4N_VARIABLE_MAN_1DINTEGER

#define OFFSET_1DInteger(x) (x*sizeof(int))

int new1DInteger(VariableObject *anker, char *group, char *name, int x_length);
int set1DIntegerX(VariableObject *anker, char *group, char *name, int x, int val);
int get1DIntegerX(VariableObject *anker, char *group, char *name, int x);
int print1DInteger(VariableObject *target, FILE *output, int mode);
#endif
