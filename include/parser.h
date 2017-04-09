#ifndef RH4N_PARSER
#define RH4N_PARSER
#include "var_management.h"

#define SAVE 1
#define END_SAVE 2
#define EXIT 3
#define CONTINUE 4
#define BREAK 5

#define LOOK_FOR_IF 1
#define LOOK_FOR_FOR 2
#define LOOK_FOR_MACRO 3

#define SIZEOF_WCHAR sizeof(wchar_t)
#define SIZEOF_CHAR sizeof(char)

#define CHECK_UNICODE_CHAR(x) x < 0x21 || x > 0x7E
#define PRINT_SYNTAX_ERROR(x) fprintf(stderr, "SyntaxError: invalid Syntax in line [%d] column [%d]\n", \
                                        x->line_no+1, x->col_no);

#define PRINT_SYNTAX_ERROR_VAR_ATTR(x) fprintf(stderr, "SyntaxError: invalid Syntax in line [%d] column [%d]\n", \
                                        x.line+1, x.start_col);

#define PRINT_UNKWON_VAR_ATTR(x) fprintf(stderr, "NameError: Unkown variable in line [%d] column [%d]\n", \
                                        x.line+1, x.start_col);

#define PRINT_UNKWON_GRP_ATTR(x) fprintf(stderr, "NameError: Unkown group in line [%d] column [%d]\n", \
                                        x.line+1, x.start_col);

#define PRINT_UNSUPPORTED_INDEX_ATTR(x) fprintf(stderr, "IndexError: Unsupported index in line [%d] column [%d]\n", \
                                        x.line+1, x.start_col);

#define UNSUPPOERTED_TYPE -1

#ifdef DEBUG_FLAG
#define D(x) if(1) x;
#else
#define D(x) if(0) x;
#endif

enum {
    COMMANDBLOCK,
    VARIABLEBLOCK,
    BLOCKEND,
    SPACE,
    CHAR,
    OPENBRACKET,
    CLOSEBRACKET,
    COMMA,
    OPENCORNERBRACKET,
    CLOSECORNERBRACKET,
    QUOTATIONMARKS,
    BACKSLASH
};

typedef struct tokenobject {
    short type;
    wchar_t val;
    int line_no;
    int col_no;
    struct tokenobject *next;
} Token_Object;

typedef struct {
    Token_Object *head;
    wchar_t **sav_buff;
    int *length;
    int real_level;
    int *cursor;

    //Depth of the command blocks
    int level;
} SaveObject;

#define FOR_INDEX 0
#define IF_INDEX 1
#define MACRO_INDEX 2

#define READ_FROM_FILE 1
#define READ_FROM_BUFFER 2
typedef struct {
    Token_Object token_tree; 
    int mode;
    int look_for;
    //Index
    //0 = in for
    //1 = in if
    //2 = in macro
    int in_status[3];

    SaveObject sav;

    int cur_col;
    int cur_line;
    int found_block;

    int read_from;
    FILE *fp;

    Token_Object *cur_token;
} ParserStatus;

int parseChr(ParserStatus *, wchar_t);
int checkBlock(ParserStatus*);
char *getFirstCommand(Token_Object**);
void getCharfromBuffer(SaveObject *sav, wchar_t *chr);
int parseStaticType(Token_Object *start, int *type, void **data);

//---------Handler functions---------
int gen_json(ParserStatus *stat);
int print_vars(ParserStatus *stat);
int dump_vars(ParserStatus *status);
int typeof_func(ParserStatus *status);

extern VariableObject *var_anker;
#endif
