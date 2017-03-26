typedef struct {
    char *attribute;
    int type;

    int index_type;
    char **index;

    int line;
    int start_col;
} VariableAttribute;

typedef struct {
    int length_of_data;
    void *data;
    int var_type;

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
