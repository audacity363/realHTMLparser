#ifndef RH4N_VARIABLE_MAN_2DSTRING
#define RH4N_VARIABLE_MAN_2DSTRING

#define OFFSET_2DString(length, y_len, x, y) ( \
    (y_len*((length+1)*sizeof(wchar_t)))* \
    (x+(((length+1)*sizeof(wchar_t))*y)))

int new2DString(VariableObject *anker, char *group, char *name, int length, int x_length, int y_length);
int set2DStringX(VariableObject *anker, char *group, char *name, int x, wchar_t *val);
wchar_t *get2DStringX(VariableObject *anker, char *group, char *name, int x, int y);
int newStringfrom2DString(VariableObject *anker, char *group, char *name, int x,
    char *new_group, char *new_name);
int print2DString(VariableObject *target, FILE *output, int mode);
int print2DStringX(VariableObject *target, int x, FILE *output);
#endif
