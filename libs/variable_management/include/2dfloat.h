#ifndef RH4N_VARIABLE_MAN_2DFLOAT
#define RH4N_VARIABLE_MAN_2DFLOAT

#define OFFSET_2DFloat(y_length, x, y) ((y_length*sizeof(double)*x)+(sizeof(double)*y))

int new2DFloat(VariableObject *anker, char *group, char *name, int x_length, int y_length);
int set2DFloatXY(VariableObject *anker, char *group, char *name, int x, int y, double val);
double get2DFloatXY(VariableObject *anker, char *group, char *name, int x, int y);
int print2DFloat(VariableObject *target, FILE *output, int mode);
#endif
