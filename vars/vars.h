#define true 1
#define false 0

#define VAR_ALREADY_DEFINED 1
#define MEMORY_ALLOC_ERROR 2
#define VAR_NOT_DEFINED 3
#define GRP_NOT_DEFINED 4
#define GRP_ALREADY_DEFINED 5
#define X_INDEX_OUT_OF_RANGE 6
#define Y_INDEX_OUT_OF_RANGE 7


struct variables_s
{
    char *name;
    void *data;
    int x_length;
    int y_length;
    int z_length;
    int type;
    struct variables_s *next_lvl;
    struct variables_s *prev;
    struct variables_s *next;
};

enum {
    GROUP,
    INTEGER,
    ONEDINTEGER,
    TWODINTEGER,
    THREEDINTEGER,
    STRING,
    ONEDSTRING,
    TWODSTRING,
    THREEDSTRING,
    BOOL,
    ONEDBOOL,
    TWODBOOL,
    THREEDBOOL
};

char *var_errorstrs[7] = 
{
    "Variable already defined",
    "Error while allocating memory",
    "Variable not defined",
    "Group not defined",
    "Group already defined",
    "X Index out of range",
    "Y Index out of range"
};

typedef struct variables_s vars_t;
typedef int bool;
