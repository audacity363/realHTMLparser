#ifndef RH4N_VARIABLE_MAN_3DBOOLEAN
#define RH4N_VARIABLE_MAN_3DBOOLEAN

#define OFFSET_3DBoolean(x_length, y_length, z_length, x, y, z) \
    ((y*(x_length*sizeof(bool))+x) * ((x_length*sizeof(bool)*y_length))*z_length + z) * sizeof(bool)

int new3DBoolean(VariableObject *anker, char *group, char *name, int x_length, int y_length, int z_length);
int set3DBooleanXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z, bool val);
bool get3DBooleanXYZ(VariableObject *anker, char *group, char *name, int x, int y, int z);
int print3DBoolean(VariableObject *target, FILE *output, int mode);

#endif
