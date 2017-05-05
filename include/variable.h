#ifndef RH4N_PARSER_VARS
#define RH4N_PARSER_VARS
typedef struct {
    char *attribute;
    int type;

    int index_type;
    char **index;

    //If the typeof the attribute is equals function the given parms
    //get saved in the parms array
    int function_parms;
    VariableObject *parms;

    int line;
    int start_col;
} VariableAttribute;

typedef struct {
    VariableObject target;

    int number_of_attributes;
    VariableAttribute *attributes;

} VariableParseData;

enum {
    FUNCTION,
    VARNAME,
    GRPNAME
};


int handle_variable(ParserStatus *status);
int getVariableAttributes(Token_Object *start, VariableParseData *var_data);
int rateAttributes(VariableParseData *var_data);
int saveAttribute(VariableParseData *data, char *buffer, int buffer_length, int line, int col);
void freeVariableData(VariableParseData *data);
int execAttributes(VariableParseData *var_data);
int parseFunctionParms(char *buffer, VariableAttribute *attr_target);
#endif
