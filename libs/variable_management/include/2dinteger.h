#ifndef RH4N_VARIABLE_MAN_2DINTEGER
#define RH4N_VARIABLE_MAN_2DINTEGER

#define OFFSET_2DInteger(y_length, x, y) ((y_length*sizeof(int)*x)+(sizeof(int)*y))

int new2DInteger(VariableObject *anker, char *group, char *name, int x_length, int y_length);
int set2DIntegerXY(VariableObject *anker, char *group, char *name, int x, int y, int val);
int get2DIntegerXY(VariableObject *anker, char *group, char *name, int x, int y);
int print2DInteger(VariableObject *target, FILE *output, int mode);
#endif
