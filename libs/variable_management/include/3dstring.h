#ifndef RH4N_VARIABLE_MAN_3DSTRING
#define RH4N_VARIABLE_MAN_3DSTRING

#define OFFSET_3DString(length, y_length, z_length, x, y, z) \
    ( \
        (x*((((length*sizeof(wchar_t))*z_length)*y_length)))+ \
        (y*((length*sizeof(wchar_t))*z_length))+ \
        (z*(length*sizeof(wchar_t))) \
    )

int new3DString(VariableObject *anker, char *group, char *name, int length, int x_length, int y_length, int z_length);
int set3DStringXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z, wchar_t *val);
wchar_t *get3DStringXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z);
int newStringfrom3DString(VariableObject *anker, char *group, char *name, int x,
    int y, int z, char *new_group, char *new_name);
int print3DString(VariableObject *target, FILE *output, int mode);
int print3DStringXYZ(VariableObject *target, int x, int y, int z, FILE *output);
#endif
