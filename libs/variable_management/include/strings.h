#ifndef RH4N_VARIABLE_MAN_STRING
#define RH4N_VARIABLE_MAN_STRING
int newString(VariableObject *anker, char *group, char *name, int length);
int setString(VariableObject *anker, char *group, char *name, wchar_t *val);
wchar_t *getString(VariableObject *anker, char *group, char *name);
int printString(VariableObject *target, FILE *output, int mode);
#endif
