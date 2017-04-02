#ifndef RH4N_VARIABLE_MAN_BOOLEAN
#define RH4N_VARIABLE_MAN_BOOLEAN

int newBoolean(VariableObject *anker, char *group, char *name);
int setBoolean(VariableObject *anker, char *group, char *name, bool val);
bool getBoolean(VariableObject *anker, char *group, char *name);
int printBoolean(VariableObject *target, FILE *output, int mode);

#endif
