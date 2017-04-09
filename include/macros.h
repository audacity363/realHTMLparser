typedef struct {
    char *name;
    int required;
    int type;

    int length_of_val;
    void *val;
} MacroParms;

typedef struct {
    char *name;

    int body_length;
    wchar_t **body;

    int number_of_parms;
    MacroParms *parms;

} MacroDefinition;

int macro_start(ParserStatus *status);
int macro_end(ParserStatus *status);
int maco_exec(ParserStatus *status);

int start_save_macro(Token_Object *start, SaveObject *sav_buff);
void printMacro(MacroDefinition *target);
