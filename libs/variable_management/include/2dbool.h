#ifndef RH4N_VARIABLE_MAN_2DBOOLEAN
#define RH4N_VARIABLE_MAN_2DBOOLEAN

#define OFFSET_2DBoolean(y_length, x, y) ((y_length*sizeof(bool)*x)+(sizeof(bool)*y))

int new2DBoolean(VariableObject *anker, char *group, char *name, int x_length, int y_length);
int set2DBooleanXY(VariableObject *anker, char *group, char *name, int x, int y, bool val);
bool get2DBooleanXY(VariableObject *anker, char *group, char *name, int x, int y);
int print2DBoolean(VariableObject *target, FILE *output, int mode);

#endif
