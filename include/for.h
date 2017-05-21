#ifndef RH4N_PARSER_FOR
#define RH4N_PARSER_FOR

typedef struct {
    int length;
    int index;

    int length_of_vars;    
    int *old_type;
    void **data_start;
    int **original_length;
    VariableParseData *vars;
} LoopProperties;


#define MY_FOR_HEAD(x) for(x.index=0;x.index<x.length;x.index++)

int for_handling(ParserStatus *);
int end_for_handling(ParserStatus *);
int exec_for(ParserStatus *status, Token_Object *start);
int parseForHead(Token_Object *start, LoopProperties *for_status);
int getForLength(LoopProperties *for_status);

int initForVariables(VariableObject *anker, LoopProperties *for_status);


int checkIfFunction(char *name, LoopProperties *for_status);
#endif
