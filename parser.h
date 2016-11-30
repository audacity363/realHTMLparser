#define true 1
#define false 0

#define open 1
#define closed 2

#define VARIABLEBLOCK 2
#define COMMANDBLOCK 3

#define COMMANDBEGIN_STR "{%"
#define COMMANDEND_STR "%}"
#define VARIABLEBEGIN_STR "{{"
#define VARIABLEEND_STR "}}"

#define FORBLOCK 100
#define IFBLOCK 200

#define JUSTSAVE 300

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
    STRING,
    EQUALS,
    GREATERTHEN,
    LESSTHEN,
    CLINGTO,
    CLAMPS
};

struct token_s
{
    char val;
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
    int sizeof_sav_buff;
    char **save_buff;
};

typedef struct token_s token_t;
typedef struct status_s status_t;
