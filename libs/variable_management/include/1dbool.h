#ifndef RH4N_VARIABLE_MAN_1DBOOLEAN
#define RH4N_VARIABLE_MAN_1DBOOLEAN

#define OFFSET_1DBoolean(x) (sizeof(bool)*x)

int new1DBoolean(VariableObject *anker, char *group, char *name, int x_length);
int set1DBooleanX(VariableObject *anker, char *group, char *name, int x, bool val);
bool get1DBooleanX(VariableObject *anker, char *group, char *name, int x);
int print1DBoolean(VariableObject *target, FILE *output, int mode);

#endif
