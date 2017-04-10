#ifndef RH4N_VARIABLE_MAN_2DSTRING
#define RH4N_VARIABLE_MAN_2DSTRING

#define OFFSET_2DString(length, y_length, x, y) \
    ( \
        ((((length+1)*sizeof(wchar_t))*y_length)*x)+ \
        (((length+1)*sizeof(wchar_t))*y) \
    )


//(y_size*x)+(var_size*y)

int new2DString(VariableObject *anker, char *group, char *name, int length, int x_length, int y_length);
int set2DStringXY(VariableObject *anker, char *group, char *name, int x, int y, wchar_t *val);
wchar_t *get2DStringXY(VariableObject *anker, char *group, char *name, int x, int y);
int newStringfrom2DString(VariableObject *anker, char *group, char *name, int x,
    char *new_group, char *new_name);
int print2DString(VariableObject *target, FILE *output, int mode);
int print2DStringXY(VariableObject *target, int x, int y, FILE *output);
#endif
