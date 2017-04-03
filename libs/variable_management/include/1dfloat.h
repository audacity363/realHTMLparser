#ifndef RH4N_VARIABLE_MAN_1DFLOAT
#define RH4N_VARIABLE_MAN_1DFLOAT

#define OFFSET_1DFloat(x) (x*sizeof(double))

int new1DFloat(VariableObject *anker, char *group, char *name, int x_length);
int set1DFloatX(VariableObject *anker, char *group, char *name, int x, double val);
double get1DFloatX(VariableObject *anker, char *group, char *name, int x);
int print1DFloat(VariableObject *target, FILE *output, int mode);
#endif
