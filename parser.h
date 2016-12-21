#define true 1
#define false 0

#define open 1
#define closed 2

#define VARIABLEBLOCK 2
#define COMMANDBLOCK 3

#define COMMANDBEGIN_STR L"{%"
#define COMMANDEND_STR L"%}"
#define VARIABLEBEGIN_STR L"{{"
#define VARIABLEEND_STR L"}}"

#define FORBLOCK 100
#define IFBLOCK 200

#define JUSTSAVE 300
#define EXIT 400

enum {
    COMMANDBEGIN,
    COMMANDEND,
    VARIABLEBEGIN,
    VARIABLEEND,
    VARIABLENAME,
    COMMANDNAME,
    INDEXOPEN,
    INDEXCLOSE,
    INDEXVAL,
    SPACE,
    CHAR,
    STR,
    EQUALS,
    GREATERTHEN,
    LESSTHEN,
    CLINGTO,
    CLAMPS,
    BLOCKSTART,
    BLOCKEND,
    CMDSTARTEND,
    COMMA
};

enum {
    IFVARIABLE,
    IFSTR,
    IFINT,
    IFCOMPARE
};

enum {
    IFLESSTHEN,
    IFLESSTHENEQUAL,
    IFEQUALS,
    IFUNEQUALS,
    IFGREATERTHEN,
    IFGREATERTHENEQUALS,
    IFIN
};

struct token_s
{
    wchar_t val;
    int type;
    struct token_s *prev;
    struct token_s *next;
};

struct status_s
{
    int lookfor;
    int just_save;
    int in_for;
    int in_if;
    int in_macro;
    size_t sizeof_sav_buff;
    wchar_t **save_buff;
};

struct if_parms_s
{
    wchar_t *val;
    int val_length;
    int type;
    int varlib_type;
    int hasindex;
    int index1d;
    int index2d;
    int index3d;
    struct if_parms_s *prev;
    struct if_parms_s *next;
};

struct macro_definition {
    char *name;
    char **value;
    int aguments_number;
    char **aguments_name;
    void **default_value;
    int *argument_type;
    struct macro_definition *next;
};

typedef struct {
    char *cur_name;
    wchar_t **macro_buff;
    int aguments_number;
    char **aguments_name;
    void **default_value;
    struct macro_definition *anker;
} macros;

typedef struct token_s token_t;
typedef struct status_s status_t;
typedef struct if_parms_s if_parms_t;

extern vars_t *vars_anker;

void freeLineBuff(status_t *stat);
int parseLine(wchar_t *line, status_t *status);
