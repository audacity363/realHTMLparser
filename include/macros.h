#ifndef RH4N_PARSER_MACROS
#define RH4N_PARSER_MACROS
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
    wchar_t *body;

    int number_of_parms;
    MacroParms *parms;

} MacroDefinition;

typedef struct {
    int length;
    MacroDefinition **macros;
} MacroEntries;

int macro_start(ParserStatus *status);
int macro_end(ParserStatus *status);
int maco_exec(ParserStatus *status);
void freeMacro(MacroDefinition **target);
void freeAllMacros(MacroEntries *target);

int start_save_macro(ParserStatus *status, Token_Object *start, SaveObject *sav_buff);
void printMacro(MacroDefinition *target);


MacroDefinition *searchMacro(MacroEntries *anker, char *name);
int execMacro(ParserStatus *status, MacroDefinition *def);
#endif
