#ifndef RH4N_VARIABLE_MAN_3DINTEGER
#define RH4N_VARIABLE_MAN_3DINTEGER

#define OFFSET_3DInteger(y_length, z_length, x, y, z) \
    ( \
        (x*(sizeof(int)*y_length))+ \
        (y*(sizeof(int)*z_length))+ \
        (z*sizeof(int)) \
    )

int new3DInteger(VariableObject *anker, char *group, char *name, int x_length, int y_length, int z_length);
int set3DIntegerXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z, int val);
int get3DIntegerXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z);
int print3DInteger(VariableObject *target, FILE *output, int mode);
#endif
