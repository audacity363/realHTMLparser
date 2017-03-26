#ifndef RH4N_VARIABLE_MAN_1DSTRING
#define RH4N_VARIABLE_MAN_1DSTRING

#define OFFSET_1DString(length, x) (((length+1)*sizeof(wchar_t))*x)

int new1DString(VariableObject *anker, char *group, char *name, int length, int x_length);
int set1DStringX(VariableObject *anker, char *group, char *name, int x, wchar_t *val);
wchar_t *get1DStringX(VariableObject *anker, char *group, char *name, int x);
int newStringfrom1DString(VariableObject *anker, char *group, char *name, int x,
    char *new_group, char *new_name);
int print1DString(VariableObject *target, FILE *output, int mode);
int print1DStringX(VariableObject *target, int x, FILE *output);
#endif
