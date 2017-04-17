#ifndef RH4N_PARSER_FOR
#define RH4N_PARSER_FOR

typedef struct {
    int length;
    int index;

    int length_of_vars;    
    VariableParseData *vars;
} LoopProperties;

int for_handling(ParserStatus *);
int end_for_handling(ParserStatus *);
int exec_for(ParserStatus *status, Token_Object *start);
int parseForHead(Token_Object *start, LoopProperties *for_status);
int checkForParms(LoopProperties *for_status);
#endif
