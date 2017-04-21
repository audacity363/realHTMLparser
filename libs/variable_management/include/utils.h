#ifndef RH4N_VARIABLE_MAN_UTILS
#define RH4N_VARIABLE_MAN_UTILS
int initAnker(VariableObject **anker);
void freeVarAnker(VariableObject *anker);
int deleteVar(VariableObject *anker, char *group, char *name);
VariableObject *getGroup(VariableObject *anker, char *group);
VariableObject *addNewVariable(VariableObject *anker, char *group, char *name, int type);
int addNewGroup(VariableObject *anker, char *group);
bool isDefinedVariable(VariableObject *anker, char *group, char *name);
VariableObject *getVariable(VariableObject *anker, char *group, char *name);
bool isDefinedGroup(VariableObject *anker, char *group);
VariableObject *getGroup(VariableObject *anker, char *group);
#endif
