#ifndef RH4N_VARIABLE_MAN_UTILS
#define RH4N_VARIABLE_MAN_UTILS
VariableObject *getGroup(VariableObject *anker, char *group);
VariableObject *addNewVariable(VariableObject *anker, char *group, char *name, int type);
bool isDefinedVariable(VariableObject *anker, char *group, char *name);
VariableObject *getVariable(VariableObject *anker, char *group, char *name);
bool isDefinedGroup(VariableObject *anker, char *group);
VariableObject *getGroup(VariableObject *anker, char *group);
#endif
