#ifndef RH4N_VARIABLE_MAN_3DFLOAT
#define RH4N_VARIABLE_MAN_3DFLOAT

#define OFFSET_3DFloat(x_length, y_length, z_length, x, y, z) \
    ( \
    (((z_length*sizeof(double)) * y_length) * x) + \
    (((z_length*sizeof(double))) * y) + \
    (z * sizeof(double)) \
    ) 

int new3DFloat(VariableObject *anker, char *group, char *name, int x_length, int y_length, int z_length);
int set3DFloatXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z, double val);
double get3DFloatXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z);
int print3DFloat(VariableObject *target, FILE *output, int mode);
#endif
